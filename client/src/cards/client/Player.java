package cards.client;
import cards.animation.Layer;
import java.util.ArrayList;
public class Player{
	public final String name;
	private int x,y;
	private int[] cards = new int[4];
	public Player(String name){
		this.name = name;
		for(int a=0; a<cards.length; a++){
			cards[a] = -1;
		}
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
	public int[] getCards(){
		return cards;
	}
	public int numberOfCards(){
		int count = 0;
		for(int a=0; a<cards.length; a++){
			if(cards[a]>=0){
				count++;
			}
		}
		return count;
	}
	public int pickRandom(){	
		if(numberOfCards()<=0){
			return -1;
		}
		int card;
		do{
			card = (int)(Math.random()*cards.length);
		}while(cards[card]<0);
		return cards[card];
	}
	public int add(int card){
		for(int a=0; a<cards.length; a++){
			if(cards[a]<0){
				cards[a] = card;
				return a;
			}
		}
		return -1;
	}
}
