package cards.client;
import javax.swing.*;
import java.awt.*;
import cards.animation.*;
import java.util.List;
import java.util.Arrays;
import java.util.ArrayList;
import static cards.client.Common.SERVER_BUNDLE;
public class GameWindow extends JFrame{
	private static final int WIDTH = 800;
	private static final int HEIGHT = 600;
	private final List<Integer> my_cards = new ArrayList<Integer>();
	private final List<Integer> my_last_cards = new ArrayList<Integer>();
	private final Client client;
	public GameWindow(Client client){
		this.client = client;
		loadLayers();
		client.addCallback("Play",(s)->{
			client.send(SERVER_BUNDLE.getString("MyCards"));
		});
		client.addCallback("Cards",(s)->{
			setCards(s);
			System.out.println("I've got cards ;)");
		});
		setDefaultCloseOperation(EXIT_ON_CLOSE); //TODO
		setSize(WIDTH,HEIGHT);
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
			PredefinedAnimations pa = new PredefinedAnimations(lm.getCard(new_cards.get(a)),this);
			pa.setPosition(WIDTH>>1+(a-2)*20,HEIGHT-100);
			pa.setRotate((a-2)*Math.PI/5);
			pa.getCard();
		}
	}
	private <T> List<T> substract(List<T> from, List<T> to){
		List<T> list = new ArrayList<T>();
		list.addAll(from);
		list.removeAll(to);
		System.out.println(Arrays.toString(from.toArray()));
		System.out.println(Arrays.toString(to.toArray()));
		System.out.println(Arrays.toString(list.toArray()));
		return list;
	}
	public void paint(Graphics g){
		LayerManager.getInstance().paint((Graphics2D)g);	
	}
}
