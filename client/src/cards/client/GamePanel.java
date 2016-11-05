package cards.client;
import javax.swing.*;
import java.awt.*;
import cards.animation.*;
import java.awt.event.*;
import java.util.List;
import java.util.Arrays;
import java.util.ArrayList;
import java.awt.image.BufferedImage;
import static java.awt.image.BufferedImage.TYPE_INT_ARGB;
import static cards.client.Common.*;
public class GamePanel extends JPanel{
	private static final int WIDTH = 800;
	private static final int HEIGHT = 600;
	private static final Point center_point = new Point(WIDTH>>1,HEIGHT>>1);
	private static final Point all_cards = new Point(WIDTH/8*3,HEIGHT>>1);
	private static final Point played_cards = new Point(WIDTH/8*5,HEIGHT>>1);
	private static final Point cards = new Point(WIDTH>>1,HEIGHT-77);
	private static final Point opponent_cards = new Point(WIDTH/6,77);
	private static final BufferedImage buffer = new BufferedImage(WIDTH,HEIGHT,TYPE_INT_ARGB);
	private final int[] my_cards = new int[4];
	private final int[] my_last_cards = new int[4];
	private final List<Player> players = new ArrayList<Player>();
	private final boolean[] card_played = new boolean[32];
	private final Client client;
	private final String name;
	private Point previous_position = null; 
	private double previous_rotate = 0;
	private int previous_card = -1;
	private boolean running_quard = true;
	private final List<Integer> cards_on_table = new ArrayList<Integer>();
	public GamePanel(Client client,String name){
		this.name = name;
		this.client = client;
		clearArray(my_cards);
		clearArray(my_last_cards);
		loadLayers();
		setFocusable(true);
		requestFocusInWindow();
		/*client.addCallback("Play",(s)->{
			//TODO
		});*/
		client.addCallback("Cards",(s)->{
			clearTable();
			setCards(s);
		});
		client.addCallback("CardCount",(s)->{
			setOponentsCards(s);
		});
		client.addCallback("CardPlayed",(s)->{
			s = s.replaceFirst("NULL","NU LL"); //HACK
			String[] split = s.split(" ");
			if(!split[2].equals(name)){
				System.out.println("opponents card");
				playOponentCard(split[0],split[1],split[2]);
			}else{
				System.out.println("my card");
				removeCard(getValue(split[0],split[1]));
			}
		});
		client.addCallback("VoteCast",(s)->{
			Object[] options = {
				BUNDLE.getString("VoteYes"),
				BUNDLE.getString("VoteNo"),
				BUNDLE.getString("VoteDontCare")
			};
			int result = JOptionPane.showOptionDialog(this,BUNDLE.getString("VotingNextGame"),
				BUNDLE.getString("Vote"),JOptionPane.YES_NO_CANCEL_OPTION,JOptionPane.QUESTION_MESSAGE,
				null,options,options[0]);
			client.send(SERVER_BUNDLE.getString("VoteResult")+" "+SERVER_BUNDLE.getString("Vote"+result));
		});
		addKeyListener(new KeyAdapter(){
			@Override
			public void keyPressed(KeyEvent e){
				client.send(SERVER_BUNDLE.getString("Play")+" "+handleKey(e.getKeyCode()));
			}
		});
		addMouseListener(new MouseAdapter(){
			@Override
			public void mousePressed(MouseEvent e){
				if(e.getButton()==MouseEvent.BUTTON1){
					client.send(SERVER_BUNDLE.getString("Play")+" "+handleMouse(e.getX(),e.getY()));
				}else if(e.getButton()==MouseEvent.BUTTON3){
					if(running_quard){
						running_quard = false;
						new Thread(()->{
							animateMouse(getMouseCard(e.getX(),e.getY()));
							running_quard = true;
						}).start();
					}
				}
			}
		});
		setPreferredSize(new Dimension(WIDTH,HEIGHT));
	}
	private void clearTable(){
		for(int index: cards_on_table){
			LayerManager lm = LayerManager.getInstance();
			Layer layer = lm.getCard(index);
			PredefinedAnimations pa = new PredefinedAnimations(layer,this);
			pa.setPosition(WIDTH+100,HEIGHT>>1);
			pa.setRotate(Math.random()*2*Math.PI);
			pa.getCard(true);
			repaint();
		}
		cards_on_table.clear();
	}
	private void animateMouse(int card){
		LayerManager lm = LayerManager.getInstance();
		if(previous_position == null){
			if(card<0){
				return;
			}
			Layer layer = lm.getCard(card);
			PredefinedAnimations pa = new PredefinedAnimations(layer,this);
			pa.setMaxZoom(5);
			pa.setPosition((int)center_point.getX(),(int)center_point.getY());
			pa.setRotate(0);
			previous_position = new Point(layer.getX(),layer.getY());
			previous_rotate = layer.getRotation();
			previous_card = card;
			pa.zoom();
		}else{
			if(card!=-2){
				return;
			}
			Layer layer = lm.getCard(previous_card);
			PredefinedAnimations pa = new PredefinedAnimations(layer,this);
			pa.setMaxZoom(1);
			pa.setPosition((int)previous_position.getX(),(int)previous_position.getY());
			pa.setRotate(previous_rotate);
			previous_position = null;
			previous_rotate = 0;
			previous_card = -1;
			pa.zoom();
		}
		repaint();
	}
	private void removeCard(int index){
		if(index<0){
			PredefinedAnimations pa = new PredefinedAnimations(null,this);
			pa.setPosition((int)cards.getX(),(int)cards.getY());
			pa.animation("DontWantToContinue");
			repaint();
		}else{
			LayerManager lm = LayerManager.getInstance();
			card_played[index] = true;
			Layer layer = lm.getCard(index);
			PredefinedAnimations pa = new PredefinedAnimations(layer,this);
			pa.setPosition((int)played_cards.getX(),(int)played_cards.getY());
			pa.setRotate(Math.random()*2*Math.PI);
			pa.getCard(true);
			repaint();
		}
	}
	private void playOponentCard(String color,String value,String player){
		int index = getValue(color,value);
		if(index<0){
			for(int a=0; a<players.size(); a++){
				Player plyr = players.get(a);
				if(plyr.name.equals(player)){
					PredefinedAnimations pa = new PredefinedAnimations(null,this);
					pa.setPosition((int)((a+0.5)*opponent_cards.getX()),(int)opponent_cards.getY());
					pa.animation("DontWantToContinue",Math.PI);
					return;
				}
			}
		}
		LayerManager lm = LayerManager.getInstance();
		Layer layer = lm.getCard(index);
		Layer to_swap = null;
		System.out.println(getValue(color,value)+": "+player);
		//get random players card
		for(Player plyr: players){
			if(plyr.name.equals(player)){
				int card = plyr.pickRandom();
				to_swap = lm.getCard(card);
				plyr.setCard(card,-1);
				to_swap.swap(layer);
				repaint();
				break;
			}
		}
		if(to_swap == null){
			return;
		}
		cards_on_table.add(index);
		PredefinedAnimations pa = new PredefinedAnimations(layer,this);
		pa.setPosition((int)played_cards.getX(),(int)played_cards.getY());
		pa.setRotate(Math.random()*2*Math.PI);
		pa.getCard(true);
		repaint();
	}
	private String handleKey(int code){
		if(code <= '0' || code > '4' || (code-'1') >= 4){
			return "NULL";
		}
		int card = my_cards[code-'1'];
		int color = (card&3);
		int value = ((card>>2)&7);
		return SERVER_BUNDLE.getString("Color"+color)+" "+SERVER_BUNDLE.getString("Value"+value);
	}
	private int getMouseCard(int x, int y){
		if(y<HEIGHT*11/15){
			return -2;
		}
		int position = 4*x/WIDTH;
		int offset = x%(WIDTH/4);
		if(offset < WIDTH/24 || offset > 5*WIDTH/24){
			return -1;
		}
		return my_cards[position];
	}

	private String handleMouse(int x, int y){
		int card = getMouseCard(x,y);
		if(card<0 || card>=32){
			return "NULL";
		}
		int color = (card&3);
		int value = ((card>>2)&7);
		return SERVER_BUNDLE.getString("Color"+color)+" "+SERVER_BUNDLE.getString("Value"+value);
	}
		
	private void loadLayers(){
		LayerManager lm = LayerManager.getInstance();
		lm.setLayers(LayerLoader.loadCards((int)all_cards.getX(),(int)all_cards.getY(),"images/cards/"));
		lm.addLayerOnBottom(LayerLoader.loadLayer("images/background/table.png"));
		lm.mergeAll();
	}
	private int getValue(String color,String rank){
		if(color.equals("NU") && rank.equals("LL")){
			return -1;
		}
		int clr = Integer.valueOf(SERVER_BUNDLE.getString(color));
		int rnk = Integer.valueOf(SERVER_BUNDLE.getString(rank));
		return (clr|(rnk<<2));
	}
	private void clearArray(int[] arr){
		for(int a=0; a<arr.length; a++){
			arr[a]=-1;
		}
	}
	public void setCards(String cards){
		System.out.println(java.util.Arrays.toString(my_last_cards));
		System.out.println(java.util.Arrays.toString(my_cards));
		clearArray(my_last_cards);
		for(int a=0; a<my_last_cards.length; a++){
			my_last_cards[a] = my_cards[a];
		}
		clearArray(my_cards);
		String[] arr = cards.split(" ");
		for(int a=1; a<arr.length; a+=2){
			int value = getValue(arr[a-1],arr[a]);
			boolean found = false;
			for(int b=0; b<my_last_cards.length; b++){
				if(value == my_last_cards[b]){
					my_cards[b] = value;
					found = true;
					break;
				}
			}
			if(!found){
				for(int b=0; b<my_cards.length; b++){
					if(my_cards[b]==-1){
						my_cards[b]=value;
						break;
					}
				}
			}
		}
		System.out.println(java.util.Arrays.toString(my_last_cards));
		System.out.println(java.util.Arrays.toString(my_cards));
		List<Integer> new_cards = substract(my_cards,my_last_cards);
		System.out.println(java.util.Arrays.toString(new_cards.toArray(new Integer[new_cards.size()])));
		LayerManager lm = LayerManager.getInstance();
		System.out.println(new_cards.size()+" new cards");
		for(int a=0; a<new_cards.size(); a++){
			int index = new_cards.get(a);
			int position = -1;
			for(int b=0; b<my_cards.length; b++){
				System.out.println("Compare: "+my_cards[b]+" with "+index);
				if(my_cards[b]==index){
					position = b;
					break;
				}
			}
			card_played[index] = true;
			Layer layer = lm.getCard(index);
			System.out.println(layer);
			PredefinedAnimations pa = new PredefinedAnimations(layer,this);
			pa.setPosition((int)this.cards.getX()+(int)((position-1.5)*200),(int)this.cards.getY());
			int ind = 0;
			while(my_cards[ind]!=index){
				ind++;
			}
			pa.setRotate((position-1.5)*Math.PI/10);
			pa.getCard(true);
		}
		repaint();
	}
	private void repaintStackOfCards(int[] cards){
		LayerManager lm = LayerManager.getInstance();
		for(int b=0; b<cards.length; b++){
			if(cards[b]>0){
				lm.pushOnTop(lm.getCard(cards[b]));
			}
		}
	}
	public void setOponentsCards(String cards){
		LayerManager lm = LayerManager.getInstance();
		String[] arr = cards.split(" ");
		int countOfPlayers = arr.length>>1;
		for(int a=1; a<arr.length; a+=2){
			int index = a>>1;
			String name = arr[a-1];
			int count = Integer.valueOf(arr[a]);
			if(players.size()<=index){
				Player play = new Player(name);
				play.setPosition((int)((index+0.5)*opponent_cards.getX()),(int)opponent_cards.getY());
				players.add(play);
			}else if(!players.get(index).name.equals(name)){
				Player play = new Player(name);
				play.setPosition((int)((index+0.5)*opponent_cards.getX()),(int)opponent_cards.getY());
				players.set(index,play);
			}
			int tmp_count = count-players.get(index).numberOfCards();
			System.out.println("Player ("+index+")"+name+" should get <"+count+" - "+players.get(index).numberOfCards()+"> "+tmp_count+" cards");
			for(int b=0,c=0; b<tmp_count; b++){
				while(card_played[c]){
					c++;
				}
				int position = players.get(index).add(c);
				card_played[c] = true;
				Layer layer = lm.getCard(c);
				PredefinedAnimations pa = new PredefinedAnimations(layer,this);
				Player player = players.get(index);
				pa.setPosition(player.getX()+(int)((position-1.5)*10),player.getY());
				pa.setRotate((position-1.5)*Math.PI/10);
				pa.getCard(false);
			}
			int[] card_array = players.get(index).getCards();
			repaintStackOfCards(card_array);
		}
	}
	private List<Integer> substract(int[] from, int[] to){
		List<Integer> list = new ArrayList<Integer>();
		for(int a=0; a<from.length; a++){
			if(from[a]<0){
				continue;
			}
			boolean found = false;
			for(int b=0; b<to.length; b++){
				if(from[a]==to[b]){
					found = true;
					break;
				}
			}
			if(!found){
				list.add(from[a]);
			}
		}
		return list;
	}
	public void paint(Graphics g){
		LayerManager.getInstance().paint(buffer.createGraphics());	
		g.drawImage(buffer,0,0,null);
	}
}
