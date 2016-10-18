package cards.client;
import javax.swing.*;
import java.awt.*;
import static cards.client.Common.BUNDLE;
import static cards.client.Common.SERVER_BUNDLE;
public class GameWindow extends JFrame{
	private static final String HEADER = "<html><head><meta charset=\"utf-8\" /></head><body>";
	private static final String FOOTER = "</body></html>";
	private final StringBuilder content = new StringBuilder();
	private final Client client;
	private final String name;
	private final JTextPane messages;
	private final JTextField message;
	public GameWindow(Client client,String name){
		this.name = name;
		this.client = client;
		Container c = getContentPane();
		c.setLayout(new BorderLayout());
		JPanel panel = new GamePanel(client,name);
		c.add(panel,BorderLayout.CENTER);
		JPanel side_panel = new JPanel(new BorderLayout());
		JPanel sub_panel = new JPanel(new BorderLayout());
		messages = new JTextPane();
		messages.setContentType("text/html");
		message = new JTextField(20);
		JButton send = new JButton(BUNDLE.getString("Send"));
		sub_panel.add(message,BorderLayout.CENTER);
		sub_panel.add(send,BorderLayout.EAST);
		side_panel.add(messages,BorderLayout.CENTER);
		side_panel.add(sub_panel,BorderLayout.SOUTH);
		c.add(side_panel,BorderLayout.EAST);
		setDefaultCloseOperation(EXIT_ON_CLOSE); //TODO
		getRootPane().setDefaultButton(send);
		pack();
		send.addActionListener((e)->{
			client.send(SERVER_BUNDLE.getString("SendMessage")+" "+message.getText());
			panel.requestFocusInWindow();
		});
		client.addCallback("ReceiveMessage",(s)->{
			String[] split = s.split(" ");
			content.append(toParagraph(split[0],split[1]));
			messages.setText(HEADER+content.toString()+FOOTER);
		});
	}
	private String toParagraph(String name,String message){
		Color color = getColor(name);
		return "<div style=\"color: rgb("+color.getRed()+", "+color.getGreen()+", "+color.getBlue()+");\">"+
			name+": "+message+
			"</div>";
	}
	private Color getColor(String name){
		float hsh = (name.hashCode()*5.1f);
		hsh-=(int)hsh;
		return Color.getHSBColor(hsh,1f,0.5f);
	}
}
		
