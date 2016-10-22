package cards.client;
public class Main{
	private static String[] parameters;
	public static void main(){
		main(parameters);
	}
	public static void main(String[] args){
		parameters = args;
		String host = "127.0.0.1";
		if(args.length>0){
			host = args[0];
		}
		new LoginWindow(host).setVisible(true);
	}
}
