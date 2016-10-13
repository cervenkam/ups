package client;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.geom.AffineTransform;
import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import static client.Common.SERVER_BUNDLE;
import static java.lang.Math.*;
import static java.awt.image.BufferedImage.TYPE_INT_ARGB;
class Position2D{
	private int x;
	private int y;
	public Position2D(int x,int y){
		this.x=x;
		this.y=y;
	}
	public int getX(){
		return x;
	}
	public int getY(){
		return y;
	}
	public void setX(int x){
		this.x=x;
	}
	public void setY(int y){
		this.y=y;
	}
}
class Card{
	private static BufferedImage not_visible_image;
	private static Component component = null;
	public final BufferedImage image;
	private final int image_width_half;
	private final int image_height_half;
	private Position2D position;
	private Runnable callback;
	boolean visible;
	public static void setNotVisibleCard(BufferedImage not){
		not_visible_image = not;
	}
	public static void setComponent(Component comp){
		component = comp;
	}
	public Card(BufferedImage image,int x,int y,boolean visible){
		this(image,new Position2D(x,y),visible);
	}
	public Card(BufferedImage image,Position2D position,boolean visible){
		this.image = image;
		this.position = position;
		this.visible = visible;
		this.image_width_half = image.getWidth()>>1;
		this.image_height_half = image.getHeight()>>1;
	}
	public Component getComponent(){
		return component;
	}
	public Position2D getPosition(){
		return position;
	}
	public void setPosition(Position2D position){
		this.position = position;
	}
	public void setPosition(int x,int y){
		position.setX(x);
		position.setY(y);
	}
	public boolean isVisible(){
		return visible;
	}
	public void setVisible(boolean visible){
		this.visible=visible;
	}
	public void paint(Graphics g){
		g.drawImage(visible?image:not_visible_image,position.getX()-image_width_half,position.getY()-image_height_half,null);
	}
	public void animation(Position2D end){
		animation(end,2000,100);
	}
	public void animation(Position2D end,int duration,int sleep){
		animation(end.getX(),end.getY(),duration,sleep);
	}
	public void animation(int end_x,int end_y){
		animation(end_x,end_y,2000,100);
	}
	public Card setCallback(Runnable callback){
		this.callback = callback;
		return this;
	}
	public Card animation(int end_x,int end_y,int duration,int sleep){
		int start_x = position.getX();
		int start_y = position.getY();
		int step_x = end_x - start_x;
		int step_y = end_y - start_y;
		long source_time = System.currentTimeMillis();
		long target_time = source_time+duration;
		final Runnable clbck = callback;
		new Thread(()->{
			long curr_time;
			while((curr_time = System.currentTimeMillis()) < target_time){
				double ratio = linearRatio(source_time,target_time,curr_time);
				position.setX(start_x+(int)(step_x*ratio));
				position.setY(start_y+(int)(step_y*ratio));
				if(component != null){
					component.repaint();
				}
				try{
					Thread.sleep(sleep);
				}catch(InterruptedException e){}
			}
			position.setX(end_x);
			position.setY(end_y);
			if(clbck != null){
				clbck.run();
			}
		}).start();
		return this;
	}
	private double linearRatio(long start,long end,long curr){
		return (double)(curr-start)/(end-start);
	}
}
public class GameWindow extends JFrame{
	public static final Position2D UNUSED_CARDS = new Position2D(400,300);
	public static final Position2D MY_HAND = new Position2D(400,450);
	public static final int MARGIN = 20;
	private final List<Integer> my_cards = new ArrayList<Integer>();
	private final List<Integer> my_last_cards = new ArrayList<Integer>();
	private final Client client;
	private Card[] cards;
	public GameWindow(Client client){
		this.client = client;
		Card.setComponent(this);
		loadImages("images/");
		client.addCallback("Play",(s)->{
			client.send(SERVER_BUNDLE.getString("MyCards"));
		});
		client.addCallback("Cards",(s)->{
			setCards(s);
			drawAnimation();
		});
	}
	public void loadImages(String path){
		cards = new Card[32];
		for(int a=0; a<32; a++){
			BufferedImage image = null;
			try{
				image = ImageIO.read(new File(path+a+".png"));
			}catch(IOException e){
				e.printStackTrace();
				continue;
			}
			cards[a] = new Card(image,UNUSED_CARDS,false);
		}
		try{
			Card.setNotVisibleCard(ImageIO.read(new File(path+"notvisible.png")));
		}catch(IOException e){
			e.printStackTrace();
			return;
		}
	}
	public void paint(Graphics g){
		for(Card card:cards){
			card.paint(g);
		}
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
	}
	private void drawAnimation(){
		for(int a=0; a<my_last_cards.size(); a++){
			int last = my_last_cards.get(a);
			boolean found = false;
			for(int b=0; b<my_cards.size(); b++){
				int now = my_cards.get(b);
				if(now == last){
					found = true;
					break;
				}
			}
			if(!found){
				trashCard(a,last);
			}
		}
		for(int a=0; a<my_cards.size(); a++){
			int now = my_cards.get(a);
			boolean found = false;
			for(int b=0; b<my_last_cards.size(); b++){
				int last = my_last_cards.get(b);
				if(now == last){
					found = true;
					break;
				}
			}
			if(!found){
				addCard(a,now);
			}
		}
	}
	private void trashCard(int position,int value){
	}
	private void addCard(int position,int value){
		cards[value].setCallback(()->{
			cards[value].setVisible(true);
		}).animation(MY_HAND.getX()+position*MARGIN,MY_HAND.getY());
	}
}
