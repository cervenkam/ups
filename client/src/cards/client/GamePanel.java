package cards.client;
import javax.swing.*;
import java.awt.*;
import cards.animation.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyAdapter;
import java.util.List;
import java.util.Arrays;
import java.util.ArrayList;
import java.awt.image.BufferedImage;
import static java.awt.image.BufferedImage.TYPE_INT_ARGB;
import static cards.client.Common.SERVER_BUNDLE;
public class GamePanel extends JPanel{
	private static final int WIDTH = 800;
	private static final int HEIGHT = 600;
	private static final Point all_cards = new Point((WIDTH>>1)-50,HEIGHT>>1);
	private static final Point played_cards = new Point((WIDTH>>1)+50,HEIGHT>>1);
	private static final Point cards = new Point(WIDTH>>1,HEIGHT-100);
	private static final Point opponent_cards = new Point(100,100);
	private static final BufferedImage buffer = new BufferedImage(WIDTH,HEIGHT,TYPE_INT_ARGB);
	private final int[] my_cards = new int[4];
	private final int[] my_last_cards = new int[4];
	private final List<Player> players = new ArrayList<Player>();
	//private final List<String> players = new ArrayList<String>();
	//private final List<Integer> player_card_count = new ArrayList<Integer>();
	private final boolean[] card_played = new boolean[32];
	private final Client client;
	private final String name;
	public GamePanel(Client client,String name){
		this.name = name;
		this.client = client;
		clearArray(my_cards);
		clearArray(my_last_cards);
		loadLayers();
		setFocusable(true);
		requestFocusInWindow();
		client.addCallback("Play",(s)->{
			client.send(SERVER_BUNDLE.getString("MyCards"));
		});
		client.addCallback("Cards",(s)->{
			setCards(s);
			client.send(SERVER_BUNDLE.getString("CardCountQuery"));
		});
		client.addCallback("CardCount",(s)->{
			setOponentsCards(s);
		});
		client.addCallback("CardPlayed",(s)->{
			String[] split = s.split(" ");
			if(!split[2].equals(name)){
				playOponentCard(split[0],split[1],split[2]);
			}else{
				removeCard(getValue(split[0],split[1]));
			}
		});
		addKeyListener(new KeyAdapter(){
			public void keyPressed(KeyEvent e){
				client.send(SERVER_BUNDLE.getString("Play")+" "+handleKey(e.getKeyCode()));
			}
		});
		setPreferredSize(new Dimension(WIDTH,HEIGHT));
	}
	private void removeCard(int index){
		LayerManager lm = LayerManager.getInstance();
		card_played[index] = true;
		Layer layer = lm.getCard(index);
		PredefinedAnimations pa = new PredefinedAnimations(layer,this);
		pa.setPosition((int)played_cards.getX(),(int)played_cards.getY());
		pa.setRotate(Math.random()*2*Math.PI);
		pa.getCard(true);
		repaint();
	}
	private void playOponentCard(String color,String value,String player){
		LayerManager lm = LayerManager.getInstance();
		int index = getValue(color,value);
		Layer layer = lm.getCard(index);
		Layer to_swap = null;
		System.out.println(getValue(color,value)+": "+player);
		//get random players card
		for(Player plyr: players){
			if(plyr.name.equals(player)){
				int card = plyr.pickRandom();
				to_swap = lm.getCard(plyr.getCards()[card]);
				plyr.getCards()[card]=-1;
				System.out.println("SWAP: "+layer+" with "+to_swap);
				to_swap.swap(layer);
			}
		}
		if(to_swap == null){
			return;
		}
		PredefinedAnimations pa = new PredefinedAnimations(to_swap,this);
		pa.setRotate(Math.random()*2*Math.PI);
		pa.setPosition((int)played_cards.getX(),(int)played_cards.getY());
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
	private void loadLayers(){
		LayerManager lm = LayerManager.getInstance();
		lm.setLayers(LayerLoader.loadCards(WIDTH>>1,HEIGHT>>1,"images/cards/"));
		//lm.addLayerOnBottom(LayerLoader.loadLayer("images/background.png"));
		lm.mergeAll();
	}
	private int getValue(String color,String rank){
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
		clearArray(my_last_cards);
		for(int a=0; a<my_last_cards.length; a++){
			my_last_cards[a] = my_cards[a];
		}
		clearArray(my_cards);
		String[] arr = cards.split(" ");
		for(int a=1; a<arr.length; a+=2){
			int value = getValue(arr[a-1],arr[a]);
			my_cards[a>>1]=value;
		}
		System.out.println(java.util.Arrays.toString(my_last_cards));
		System.out.println(java.util.Arrays.toString(my_cards));
		List<Integer> new_cards = substract(my_cards,my_last_cards);
		List<Integer> old_cards = substract(my_last_cards,my_cards);
		System.out.println(java.util.Arrays.toString(new_cards.toArray(new Integer[new_cards.size()])));
		System.out.println(java.util.Arrays.toString(old_cards.toArray(new Integer[old_cards.size()])));
		LayerManager lm = LayerManager.getInstance();
		System.out.println(new_cards.size()+" new cards");
		for(int a=0; a<new_cards.size(); a++){
			int index = new_cards.get(a);
			int position = 0;
			for(int b=0; b<my_cards.length; b++){
				if(my_cards[b]==index){
					position = b;
					break;
				}
			}
			card_played[index] = true;
			Layer layer = lm.getCard(index);
			System.out.println(layer);
			PredefinedAnimations pa = new PredefinedAnimations(layer,this);
			pa.setPosition((int)this.cards.getX()+(int)((a-1.5)*20),(int)this.cards.getY());
			int ind = 0;
			while(my_cards[ind]!=index){
				ind++;
			}
			pa.setRotate((position-1.5)*Math.PI/10);
			pa.getCard(true);
			repaint();
		}
		System.out.println(old_cards.size()+" old cards");
		for(int a=0; a<old_cards.size(); a++){
			int index = old_cards.get(a);
			removeCard(index);
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
				play.setPosition((index+1)*(int)opponent_cards.getX(),(int)opponent_cards.getY());
				players.add(play);
			}else if(!players.get(index).name.equals(name)){
				Player play = new Player(name);
				play.setPosition((index+1)*(int)opponent_cards.getX(),(int)opponent_cards.getY());
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
				pa.setPosition(player.getX()+(int)(b-(count/2)-0.5)*20,player.getY());
				pa.setRotate(((count/2)-b-0.5)*Math.PI/10);
				pa.getCard(false);
			}
			int[] card_array = players.get(index).getCards();
			for(int b=0; b<card_array.length; b++){
				if(card_array[b]>0){
					lm.pushOnTop(lm.getCard(card_array[b]));
				}
			}
			repaint();
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
