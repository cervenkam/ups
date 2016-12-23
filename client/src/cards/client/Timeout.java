package cards.client;
import static cards.client.Common.*;
public class Timeout extends Thread{
	private int time;
	private Client client;
	private boolean ok = true;
	private boolean running = true;
	private boolean onstoprunning = false;
	private Runnable onstop = null;
	private String message;
	private static final Timeout timeout = new Timeout();
	public static Timeout getTimeout(){
		return timeout;
	}
	private Timeout(){
		start();
	}
	public void setTime(int time){
		this.time = time;
	}
	public void setMessage(String message){
		this.message = message;
	}
	public boolean isOnStopRunning(){
		return onstoprunning;
	}
	public void setClient(Client client){
		this.client = client;
	}
	public void setOnStop(Runnable onstop){
		this.onstop = onstop;
	}
	public void run(){
		while(running){
			try{
				sleep(time);
			}catch(InterruptedException e){}
			if(!ok && onstop!=null){
				System.out.println("RUNNING");
				onstoprunning = true;
				onstop.run();
				onstop = null;
				onstoprunning = false;
				System.out.println("NOT RUNNING");
			}
			ok=false;
			try{
				client.send(message);
			}catch(Exception e){
				ok = true;
			}
		}
	}
	public void stopTimeout(){
		running = false;
	}
	public void itIsOk(){
		ok = true;
	}
}
