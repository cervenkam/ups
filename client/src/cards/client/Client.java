package cards.client;
import java.net.InetAddress;
import java.net.Socket;
import java.net.ConnectException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Map;
import java.util.Set;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.function.Consumer;
import static cards.client.Common.*;
public class Client extends Thread{
	private static final byte[] buffer = new byte[1024];
	private String host;
	private int port;
	private boolean running = true;
	private Map<String,Consumer<String>> callbacks = new HashMap<>();
	private OutputStream out;
	public Client(String host,int port){
		this.host = host;
		this.port = port;
	}
	public void addCallback(String str,Consumer<String> func){
		callbacks.put(SERVER_BUNDLE.getString(str),func);
	}
	public void removeCallback(String command){
		command = SERVER_BUNDLE.getString(command);
		Set<Entry<String,Consumer<String>>> set = callbacks.entrySet();
		for(Entry<String,Consumer<String>> ent: set){
			if(command.startsWith(ent.getKey())){
				callbacks.remove(ent.getKey());
				break;
			}
		}
	}
	public void send(String message){
		try{
			out.write(htonl(message.length()));
			out.write(message.getBytes());
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	public void run(){
		running = true;
		try{
			InetAddress address = InetAddress.getByName(host);
			System.out.println(address);
			Socket socket;
			try{
				socket = new Socket(address, port);
			}catch(ConnectException e){
				running = false;
				error("ErrorConnection");
				return;
			}
			out = socket.getOutputStream();
			InputStream in = socket.getInputStream();
			while(running){
				int msglen = 0;
				for(char a=0; a<4; a++){ //32bit number
					msglen<<=8;
					msglen|=in.read();
				}
				int count = in.read(buffer,0,msglen);
				if(count==msglen){
					String data = new String(buffer, 0, count);
					System.out.println("Received: "+data); //TODO lang
					notifyObserver(data);
				}else{
					System.err.println("Bad message size: "+msglen); //TODO lang
					System.err.println(new String(buffer, 0 ,count)); //TODO dangerous
				}
			}
		}catch(Exception e){
			error("ErrorGeneral");
			e.printStackTrace();
		}
		running = false;
	}
	private void softstop(){
		running = false;
	}
	private void notifyObserver(String command){
		Set<Entry<String,Consumer<String>>> set = callbacks.entrySet();
		for(Entry<String,Consumer<String>> ent: set){
			//System.out.println("\""+command+"\".startsWith(\""+ent.getKey()+"\")");
			//System.out.println("\""+Arrays.toString(command.getBytes())+"\".startsWith(\""+Arrays.toString(ent.getKey().getBytes())+"\")");
			if(command.startsWith(ent.getKey())){
				//System.out.println("OK");
				ent.getValue().accept(removeStart(command,ent.getKey()));
				break;
			}
		}
	}
	private String removeStart(String str,String start){
		int pos = str.indexOf(start);
		if(pos==0 && start.length()<str.length()){
			return str.substring(start.length()+1);
		}else{
			return "";
		}
	}
}
