package cards.client;
import java.awt.Dimension;
import java.awt.Component;
import java.util.ResourceBundle;
import java.util.Locale;
import java.nio.ByteOrder;
import java.nio.ByteBuffer;
import java.awt.event.WindowListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;
import javax.swing.AbstractButton;
import static java.awt.EventQueue.invokeLater;
import static javax.swing.JOptionPane.showMessageDialog;
import static javax.swing.JOptionPane.ERROR_MESSAGE;
public class Common{
	public static final int INSETS = 10;
	public static final ResourceBundle SERVER_BUNDLE = ResourceBundle.getBundle("bundles/Server"  ,new Locale("cs","CZ"));
	public static final ResourceBundle BUNDLE        = ResourceBundle.getBundle("bundles/Language",new Locale("cs","CZ"));
	public static final boolean reverse = ByteOrder.nativeOrder().equals(ByteOrder.BIG_ENDIAN);
	public static void standardHandle(Client client,AbstractButton... buttons){
		Timeout.getTimeout().setOnStop(()->{
			error("ErrorConnection");
			for(AbstractButton button: buttons){
				button.setEnabled(true);
			}
		});
		client.setOnStop(()->{
			error("ErrorConnection");
			for(AbstractButton button: buttons){
				button.setEnabled(true);
			}
		});
	}
	public static Dimension setPosition(boolean right,Component c,int x,int y){
		Dimension size = c.getPreferredSize();
		c.setBounds(x-(right?size.width:0),y,size.width,size.height);
		return new Dimension(x+(right?0:size.width)+INSETS,y+size.height+INSETS);
	}
	public static void error(String message){
		String messg = BUNDLE.getString(message);
		String error = BUNDLE.getString("Error");
		//invokeLater(()->{
			showMessageDialog(null,messg,error,ERROR_MESSAGE);
		//});
	}
	public static byte[] htonl(int val){
		if(reverse){
			val = Integer.reverseBytes(val);
		}
		return ByteBuffer.allocate(4).putInt(val).array();
	}
	public static WindowListener getWindowListener(Client client){
		return new WindowAdapter(){
			public void windowClosing(WindowEvent e){
				client.addCallback("Bye",(s)->{
					System.exit(0);
				});
				client.send(SERVER_BUNDLE.getString("Disconnect"));
				new Thread(()->{
					try{
						Thread.sleep(3000);
					}catch(InterruptedException f){}
					System.exit(0);
				}).start();
			}
		};
	}
}
