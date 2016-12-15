package cards.client;
import javax.swing.*;
import java.awt.*;
import javax.swing.border.EmptyBorder;
import static cards.client.Common.*;
public class GameWindow extends JFrame{
	private static final String HEADER = "<html><head><meta charset=\"utf-8\" /></head><body>";
	private static final String FOOTER = "</body></html>";
	private final Client client;
	private final String name;
	private final JTextPane messages;
	private final JTextField message;
	private String content = "";
	public GameWindow(Client client,String name){
		this.name = name;
		this.client = client;
		Container c = getContentPane();
		c.setLayout(new BorderLayout());
		setTitle(BUNDLE.getString("GameWindow"));
		JPanel panel = new GamePanel(client,name);
		panel.setBorder(new EmptyBorder(10,10,10,10));
		JPanel side_panel = new JPanel(new BorderLayout());
		side_panel.setBorder(new EmptyBorder(10,10,10,10));
		JPanel sub_panel = new JPanel(new BorderLayout());
		messages = new JTextPane();
		messages.setBackground(null);
		messages.setEditable(false);
		messages.setContentType("text/html");
		message = new JTextField(20);
		JButton send = new JButton(BUNDLE.getString("Send"));
		sub_panel.add(message,BorderLayout.CENTER);
		sub_panel.add(send,BorderLayout.EAST);
		side_panel.add(messages,BorderLayout.CENTER);
		side_panel.add(sub_panel,BorderLayout.SOUTH);
		c.add(side_panel,BorderLayout.EAST);
		c.add(panel,BorderLayout.CENTER);
		getRootPane().setDefaultButton(send);
		panel.setFocusable(true);
		panel.requestFocusInWindow();
		pack();
		send.addActionListener((e)->{
			client.send(SERVER_BUNDLE.getString("SendMessage")+" "+message.getText());
		});
		client.addCallback("ReceiveMessage",(s)->{
			String[] split = s.split(" ");
			content = toParagraph(split[0],split[1]) + content;
			messages.setText(HEADER+content+FOOTER);
		});
		client.addCallback("Bye",(s)->{
			error("ErrorDisconnected");
			dispose();
			Main.main();
		});
		addWindowListener(Common.getWindowListener(client));
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
		
