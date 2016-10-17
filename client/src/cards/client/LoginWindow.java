package cards.client;
import javax.swing.*;
import java.util.*;
import java.awt.Insets;
import java.awt.Dimension;
import java.awt.Container;
import java.awt.Component;
import java.awt.event.*;
import javax.swing.border.EmptyBorder;
import static cards.client.Common.*;
import static javax.swing.JSlider.HORIZONTAL;
import static java.util.ResourceBundle.getBundle;
import static javax.swing.SwingUtilities.invokeLater;
public class LoginWindow extends JFrame implements Runnable{
	private String host;
	public LoginWindow(String host){
		this.host = host;
		invokeLater(this);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
	}
	public void run(){
		createGUI();
	}
	private void createGUI(){
		//initial settings
		Container c = getContentPane();
		c.setLayout(null);
		//create section
		JLabel connection_server_label = new JLabel(BUNDLE.getString("Server")+":");
		JTextField connection_server =   new JTextField(20);
		connection_server.setText(host);
		JLabel connection_port_label =   new JLabel(BUNDLE.getString("Port")+":");
		JTextField connection_port =     new JTextField(5);
		connection_port.setText("43158"); //TODO debug (maybe)
		JLabel connection_name_label =   new JLabel(BUNDLE.getString("Name")+":");
		JTextField connection_name =     new JTextField(20);
		connection_name.setText(System.getProperty("user.name"));
		JButton connection_button =      new JButton(BUNDLE.getString("Login"));
		//positioning
				setPosition(false,connection_server_label, 10,10);
				setPosition(true, connection_server,      300,10);
				setPosition(false,connection_port_label,   10,30);
				setPosition(true, connection_port,        300,30);
				setPosition(false,connection_name_label,   10,50);
				setPosition(true, connection_name,        300,50);
		Dimension d =	setPosition(true, connection_button,      300,70);
		//add section
		c.add(connection_server_label);
		c.add(connection_server);
		c.add(connection_port_label);
		c.add(connection_port);
		c.add(connection_name_label);
		c.add(connection_name);
		c.add(connection_button);
		//size
		setSize(d);
		//onclick
		getRootPane().setDefaultButton(connection_button);
		connection_button.addActionListener((x)->{
			int port = -1;
			try{
				port = Integer.parseInt(connection_port.getText());
				if(port >= 1<<16){
					error("ErrorPortLarge");
					return;
				}
				if(port < 0){
					error("ErrorPortNegative");
					return;
				}
			}catch(NumberFormatException e){
				error("ErrorPortNumber");
				return;
			}catch(Exception e){
				error("ErrorGeneral");
				e.printStackTrace();
				return;
			}
			Client client = new Client(connection_server.getText(),port);
			client.addCallback("Welcome",(s)->{
				client.removeCallback("Welcome");
				client.send(SERVER_BUNDLE.getString("Login")+" "+connection_name.getText());
			});
			client.addCallback("NoGame",(s)->{
				client.removeCallback("NoGame");
				dispose();
				String name = connection_name.getText();
				new CreateGameWindow(client,name).setVisible(true);
			});
			client.addCallback("Game",(s)->{
				client.removeCallback("Game");
				dispose();
				new GameWindow(client).setVisible(true);
			});
			client.addCallback("PlayerExists",(s)->{
				error("ErrorPlayerExists");
			});
			client.start();
		});
	}
}
