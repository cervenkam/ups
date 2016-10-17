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
public class GameWindow extends JFrame{
	private static final int WIDTH = 800;
	private static final int HEIGHT = 600;
	private static final BufferedImage buffer = new BufferedImage(WIDTH,HEIGHT,TYPE_INT_ARGB);
	private final List<Integer> my_cards = new ArrayList<Integer>();
	private final List<Integer> my_last_cards = new ArrayList<Integer>();
	private final List<String> players = new ArrayList<String>();
	private final boolean[] card_played = new boolean[32];
	private final Client client;
	public GameWindow(Client client){
		this.client = client;
		loadLayers();
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
		addKeyListener(new KeyAdapter(){
			public void keyPressed(KeyEvent e){
				client.send(SERVER_BUNDLE.getString("Play")+" "+handleKey(e.getKeyCode()));
			}
		});
		setDefaultCloseOperation(EXIT_ON_CLOSE); //TODO
		setSize(WIDTH,HEIGHT);
	}
	private String handleKey(int code){
		if(code <= '0' || code > '4' || (code-'1') >= my_cards.size()){
			return "NULL";
		}
		int card = my_cards.get(code-'1');
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
	public void setCards(String cards){
		my_last_cards.clear();
		for(Integer i:my_cards){
			my_last_cards.add(i);
		}
		my_cards.clear();
		String[] arr = cards.split(" ");
		for(int a=1; a<arr.length; a+=2){
			int color = Integer.valueOf(SERVER_BUNDLE.getString(arr[a-1]));
			int rank = Integer.valueOf(SERVER_BUNDLE.getString(arr[a]));
			int value = (color|(rank<<2));
			my_cards.add(value);
		}
		List<Integer> new_cards = substract(my_cards,my_last_cards);
		List<Integer> old_cards = substract(my_last_cards,my_cards);
		LayerManager lm = LayerManager.getInstance();
		System.out.println(new_cards.size()+" new cards");
		for(int a=0; a<new_cards.size(); a++){
			int index = new_cards.get(a);
			card_played[index] = true;
			Layer layer = lm.getCard(index);
			System.out.println(layer);
			PredefinedAnimations pa = new PredefinedAnimations(layer,this);
			pa.setPosition((WIDTH>>1)+(int)((a-1.5)*20),HEIGHT-100);
			pa.setRotate((a-1.5)*Math.PI/10);
			pa.getCard(true);
			repaint();
		}
		for(int a=0; a<old_cards.size(); a++){
			int index = old_cards.get(a);
			card_played[index] = true;
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
				players.add(name);
			}else if(!players.get(index).equals(name)){
				players.set(index,name);
			}
			System.out.println("Player "+name+" has "+count+" cards");
			for(int b=0,c=0; b<count; b++){
				while(card_played[c]){
					c++;
				}
				Layer layer = lm.getCard(c);
				card_played[c] = true;
				PredefinedAnimations pa = new PredefinedAnimations(layer,this);
				pa.setPosition(((index*(WIDTH-120)/countOfPlayers>>1)+60)+(int)(b-(count/2)-0.5)*20,40);
				pa.setRotate(((count/2)-b-0.5)*Math.PI/10);
				pa.getCard(false);
			}
		}
	}
	private <T> List<T> substract(List<T> from, List<T> to){
		List<T> list = new ArrayList<T>();
		list.addAll(from);
		list.removeAll(to);
		return list;
	}
	public void paint(Graphics g){
		LayerManager.getInstance().paint(buffer.createGraphics());	
		g.drawImage(buffer,0,0,null);
	}
}
