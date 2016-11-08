package cards.client;
import javax.swing.*;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;
import java.util.List;
import java.util.ArrayList;
import java.awt.Container;
import java.awt.Dimension;
import javax.swing.border.EmptyBorder;
import static cards.client.Common.*;
import static javax.swing.JSlider.HORIZONTAL;
import static javax.swing.SwingUtilities.invokeLater;
class CreateGameWindow extends JFrame implements Runnable{

	private static final String[] algorithms = {"NetworkPlayer","ProgrammerBot","Person","AlgoNo"};
	private static final int MAX_PLAYERS = 6;
	private static final int MIN_PLAYERS = 2;
	private final List<JComboBox<String>> players_algorithms = new ArrayList<JComboBox<String>>();
	private final List<JTextField> players_names = new ArrayList<JTextField>();
	private JTextField create_name;
	private JSlider create_players_count;
	private Client client;
	private String name;
	
	public CreateGameWindow(Client client,String name){
		this.name=name;
		this.client=client;
		invokeLater(this);
		addWindowListener(Common.getWindowListener(client));
	}
	public void run(){
		createGUI();
	}
	private void createGUI(){
		//initial settings
		Container c = getContentPane();
		c.setLayout(null);
		setTitle(BUNDLE.getString("CreateGameWindow"));
		//create section
		JLabel create_name_label = new JLabel(BUNDLE.getString("GameName")+":");
		create_name = new JTextField(20);
		create_name.setText("hra "+(int)(900*Math.random()+100));//TODO DEBUG
		JLabel create_players_count_label = new JLabel(BUNDLE.getString("PlayersCount")+":");
		create_players_count = new JSlider(HORIZONTAL,MIN_PLAYERS,MAX_PLAYERS,MIN_PLAYERS);
		JButton create_players_button = new JButton(BUNDLE.getString("CreateGame"));
		//positioning
				setPosition(false,create_name_label,          10,10               );
				setPosition(true, create_name,               400,10               );
				setPosition(false,create_players_count_label, 10,30               );
				setPosition(true, create_players_count,      400,30               );
		Dimension d =	setPosition(true, create_players_button,     400,50+60*MAX_PLAYERS);
		//add section
		c.add(create_name_label);
		c.add(create_name);
		c.add(create_players_count_label);
		c.add(create_players_count);
		createPlayers(c);
		c.add(create_players_button);
		
		//init section
		create_players_count.addChangeListener(new ChangeListener(){
			public void stateChanged(ChangeEvent e){
				refreshPlayers((int)create_players_count.getValue());
			}
		});
		client.addCallback("Bye",(s)->{
			error("ErrorDisconnected");
			dispose();
			Main.main();
		});
		getRootPane().setDefaultButton(create_players_button);
		refreshPlayers((int)create_players_count.getValue());
		//border section
		c.setPreferredSize(d);
		pack();
		create_players_button.addActionListener((x)->{
			String command = getCommand();
			client.send(command);
		});
		client.addCallback("GameCreated",(e)->{
			client.removeCallback("GameCreated");
			client.send(SERVER_BUNDLE.getString("Login")+" "+name);
		});
		client.addCallback("Game",(e)->{
			client.removeCallback("Game");
			dispose();
			new GameWindow(client,name).setVisible(true);
		});
	}
	private String getCommand(){
		int count = (int)create_players_count.getValue();
		StringBuilder sb = new StringBuilder();
		sb.append(SERVER_BUNDLE.getString("CreateGame")).append(" ");
		sb.append(create_name.getText()).append(" ");
		sb.append(SERVER_BUNDLE.getString("NumberPlayers")).append("=").append(count);
		for(int a=0; a<count; a++){
			sb.append(" ").append(SERVER_BUNDLE.getString("Name")).append("=");
			sb.append(players_names.get(a).getText());
			sb.append(" ").append(SERVER_BUNDLE.getString("Player")).append("=");
			sb.append(players_algorithms.get(a).getSelectedItem().toString());
		}
		String result = sb.toString();
		System.out.println(result);
		return result;
	}
	private void refreshPlayers(int players){
		for(int a=1; a<MAX_PLAYERS; a++){
			players_algorithms.get(a).setEnabled(a<players);
			players_names.get(a).setEditable(a<players);
		}
	}
	private void createPlayers(Container con){
		String[] names = {"karel","franta","josef","venca","radek","richard"};//TODO debug
		for(int a=0; a<MAX_PLAYERS; a++){
			JLabel algorithm_label = new JLabel(BUNDLE.getString("PlayerType")+":");
			JComboBox<String> algorithm = new JComboBox<String>(algorithms);
			JLabel name_label = new JLabel(BUNDLE.getString("Name")+":");
			JTextField name = new JTextField(20);
			name.setText(names[a]);//TODO debug
			players_algorithms.add(algorithm);
			players_names.add(name);
			//add section
			con.add(algorithm_label);
			con.add(algorithm);
			con.add(name_label);
			con.add(name);
			//positioning
			int pos = 50+60*a;
			setPosition(false,algorithm_label, 10,pos   );
			setPosition(true, algorithm,      400,pos   );
			setPosition(false,name_label,      10,pos+35);
			setPosition(true, name,           400,pos+35);
			if(a==0){
				name.setText(this.name);
				name.setEditable(false);
				algorithm.setEnabled(false);
			}else{
				algorithm.setSelectedItem("AlgoNo");
			}
		}
	}
}
