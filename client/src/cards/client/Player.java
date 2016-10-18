package cards.client;
import cards.animation.Layer;
import java.util.ArrayList;
public class Player extends ArrayList<Layer>{
	public final String name;
	private int x,y;
	public Player(String name){
		this.name = name;
	}
	public void setPosition(int x,int y){
		this.x=x;
		this.y=y;
	}
	public int getX(){
		return x;
	}
	public int getY(){
		return y;
	}
}
