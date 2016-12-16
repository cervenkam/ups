package cards.client;
import static cards.client.Common.*;
public class Timeout<T> extends Thread{
	private final int time;
	private final Client client;
	private boolean ok = false;
	private boolean running = true;
	public Timeout(int time,Client client){
		this.client=client;
		this.time=time;
	}
	public void run(){
		while(running){
			try{
				client.send("I AM HERE");
			}catch(Exception e){
				ok = true;
			}
			try{
				sleep(time);
			}catch(InterruptedException e){}
			if(!ok){
				error("ErrorConnection");
			}
			ok=false;	
		}
	}
	public void stopTimeout(){
		running = false;
	}
	public void itIsOk(){
		ok = true;
	}
}
