package cards.client;
import java.net.InetAddress;
import java.net.Socket;
import java.net.ConnectException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
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
	public synchronized void addCallback(String str,Consumer<String> func){
		callbacks.put(SERVER_BUNDLE.getString(str),func);
	}
	public synchronized void removeCallback(String command){
		command = SERVER_BUNDLE.getString(command);
		Set<Entry<String,Consumer<String>>> set = callbacks.entrySet();
		for(Entry<String,Consumer<String>> ent: set){
			if(command.equals(ent.getKey())){
				callbacks.remove(ent.getKey());
				break;
			}
		}
	}
	public void send(String message){
		try{
			out.write((message+'\n').getBytes());
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
				int count = in.read(buffer,0,buffer.length);
				if(count<0){
					System.out.println("EOS");
					break;
				}else if(count<buffer.length){
					BufferedReader br = new BufferedReader(new InputStreamReader(new ByteArrayInputStream(buffer,0,count)));
					String data;
					while((data = br.readLine()) != null){
						System.out.println("Received: "+data); //TODO lang
						notifyObserver(data);
					}
					br.close();
				}else{
					System.err.println("Bad message size - more than "+buffer.length); //TODO lang
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
	private synchronized void notifyObserver(String command){
		Set<Entry<String,Consumer<String>>> set = callbacks.entrySet();
		for(Entry<String,Consumer<String>> ent: set){
			//System.out.println("\""+command+"\".startsWith(\""+ent.getKey()+"\")");
			System.out.println("\""+Arrays.toString(command.getBytes())+"\".startsWith(\""+Arrays.toString(ent.getKey().getBytes())+"\")");
			if(command.startsWith(ent.getKey())){
				//System.out.println("OK");
				try{
					ent.getValue().accept(removeStart(command,ent.getKey()));
				}catch(Exception e){
					System.err.println("Bad parameters for command \""+ent.getKey()+"\" :"+removeStart(command,ent.getKey()));
					e.printStackTrace();
				}
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
