package client;
import java.awt.Dimension;
import java.awt.Component;
import java.util.ResourceBundle;
import java.util.Locale;
import java.nio.ByteOrder;
import java.nio.ByteBuffer;
import static javax.swing.JOptionPane.showMessageDialog;
import static javax.swing.JOptionPane.ERROR_MESSAGE;
public class Common{
	public static final int INSETS = 10;
	public static final ResourceBundle SERVER_BUNDLE = ResourceBundle.getBundle("bundles/Server"  ,new Locale("cs","CZ"));
	public static final ResourceBundle BUNDLE        = ResourceBundle.getBundle("bundles/Language",new Locale("cs","CZ"));
	public static final boolean reverse = ByteOrder.nativeOrder().equals(ByteOrder.BIG_ENDIAN);
	public static Dimension setPosition(boolean right,Component c,int x,int y){
		Dimension size = c.getPreferredSize();
		c.setBounds(x-(right?size.width:0),y,size.width,size.height);
		return new Dimension(x+(right?0:size.width)+INSETS,y+size.height+INSETS);
	}
	public static void error(String message){
		showMessageDialog(null,BUNDLE.getString(message),BUNDLE.getString("Error"),ERROR_MESSAGE);
	}
	public static byte[] htonl(int val){
		if(reverse){
			val = Integer.reverseBytes(val);
		}
		return ByteBuffer.allocate(4).putInt(val).array();
	}
}
