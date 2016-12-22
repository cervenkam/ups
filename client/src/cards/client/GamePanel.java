package cards.client;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
import java.util.Arrays;
import java.util.ArrayList;
import java.awt.image.BufferedImage;
import java.util.function.BiConsumer;
import static cards.client.Common.*;
import static java.awt.event.KeyEvent.*;
import static javax.swing.JOptionPane.showMessageDialog;
import static javax.swing.JOptionPane.INFORMATION_MESSAGE;
public class GamePanel extends JPanel{
	private Client client;
	private String name;
	private JLabel ontable[] = new JLabel[16];
	private JLabel players[] = new JLabel[4];
	private JButton[] cardsbuttons = new JButton[4];
	private JButton dont_want;
	private ImageIcon cards[] = new ImageIcon[32];
	public GamePanel(Client client,String name){
		this.client = client;
		this.name = name;
		setLayout(new BorderLayout(5,5));
		prepareCards();
		JPanel ontab = prepareOnTableStack();
		JPanel cntrl = prepareControls();
		JPanel plyrs = prepareOpponents();
		add(ontab,BorderLayout.CENTER);
		add(cntrl,BorderLayout.SOUTH);
		add(plyrs,BorderLayout.EAST);
		client.addCallback("OnTable",(s)->{
			updateOnTableStack(s);
		});
		client.addCallback("CardCount",(s)->{
			updatePlayers(s);
		});
		client.addCallback("Cards",(s)->{
			updateCards(s);
		});
		client.addCallback("Play",(s)->{
			notifyPlayer();
		});
		client.addCallback("VoteCast",(s)->{
			vote();
		});
		client.addCallback("GameStatus",(s)->{
			gameStatus(s);
		});
		setPreferredSize(new Dimension(700,300));
		client.send(SERVER_BUNDLE.getString("MyCards"));
		client.send(SERVER_BUNDLE.getString("CardCountQuery"));
		client.send(SERVER_BUNDLE.getString("OnTableQuery"));
	}
	public void prepareCards(){
		String text="0";
		for(int a=0; a<cards.length; a++){
			if(a==10){
				text="";
			}
			cards[a] = new ImageIcon("images/cards/"+text+a+".png");
		}
	}
	public void gameStatus(String message){
		showMessageDialog(null,BUNDLE.getString("You")+" "+message,BUNDLE.getString("GameStatus"),INFORMATION_MESSAGE);
	}
	public void vote(){
		Object[] options = {
			BUNDLE.getString("VoteYes"),
			BUNDLE.getString("VoteNo"),
			BUNDLE.getString("VoteDontCare")
		};
		int result = JOptionPane.showOptionDialog(this,BUNDLE.getString("VotingNextGame"),
			BUNDLE.getString("Vote"),JOptionPane.YES_NO_CANCEL_OPTION,JOptionPane.QUESTION_MESSAGE,
			null,options,options[0]);
		client.send(SERVER_BUNDLE.getString("VoteResult")+" "+SERVER_BUNDLE.getString("Vote"+result));
	}
	public void prepareKeyListener(){
		addKeyListener(new KeyAdapter(){
			public void keyPressed(KeyEvent e){
				int button = -1;
				switch(e.getKeyCode()){
					case VK_A: button=0; break;
					case VK_S: button=1; break;
					case VK_D: button=2; break;
					case VK_F: button=3; break;
					case VK_SPACE:
					case VK_G: button=10; break;
				}
				if(button >= 0){
					if(button<cardsbuttons.length){
						cardsbuttons[button].doClick();
					}else{
						dont_want.doClick();
					}
				}
			}
		});
	}
	public JPanel prepareControls(){
		JPanel controls = new JPanel(new GridLayout(1,0));
		for(int a=0; a<cardsbuttons.length; a++){
			final JButton tmp = new JButton("");
			tmp.addActionListener((e)->{
				enableButtons(false);
				client.send(SERVER_BUNDLE.getString("Play")+" "+tmp.getText());
				Common.standardHandle(client,tmp,dont_want);
			});
			controls.add(tmp);
			cardsbuttons[a] = tmp;
		}
		dont_want = new JButton(BUNDLE.getString("DontWant"));
		controls.add(dont_want);
		dont_want.addActionListener((e)->{
			enableButtons(false);
			client.send(SERVER_BUNDLE.getString("Play")+" NULL");
			Common.standardHandle(client,dont_want);
		});
		enableButtons(false);
		prepareKeyListener();		
		return controls;
	}
	public JPanel prepareOpponents(){
		return prepareArray(players);
	}
	public JPanel prepareOnTableStack(){
		return prepareArray(ontable);
	}
	public JPanel prepareArray(JLabel[] arr){
		JPanel pane = new JPanel(new GridLayout(0,1));
		for(int a=0; a<arr.length; a++){
			arr[a] = new JLabel("");
			pane.add(arr[a]);
		}
		return pane;
	}
	public synchronized void enableButtons(boolean enable){
		for(int a=0; a<cardsbuttons.length; a++){
			if(!enable || !cardsbuttons[a].getText().isEmpty()){
				cardsbuttons[a].setEnabled(enable);
			}
		}
		dont_want.setEnabled(enable);
	}
	public int cardGetIndex(String name){
		String[] params = name.split(" ");
		return Integer.valueOf(SERVER_BUNDLE.getString(params[0]))
			| (Integer.valueOf(SERVER_BUNDLE.getString(params[1])) << 2);
	}
	public void updateCards(String message){
		updateArray(message,cardsbuttons,(b,s)->{
			if(s.isEmpty()){
				b.setEnabled(false);
			}
			b.setText(s);
			if(!s.isEmpty()){
				b.setIcon(cards[cardGetIndex(s)]);
			}else{
				b.setIcon(null);
			}
			b.setVerticalTextPosition(JLabel.BOTTOM);
			b.setHorizontalTextPosition(JLabel.CENTER);
		});
	}
	public void notifyPlayer(){
		enableButtons(true);
	}
	public void updateOnTableStack(String message){
		updateArray(message,ontable,(l,s)->{
			l.setText(s);
			if(!s.isEmpty()){
				l.setIcon(cards[cardGetIndex(s)]);
			}else{
				l.setIcon(null);
			}
		});
	}
	public void updatePlayers(String message){
		updateArray(message,players,(l,s)->{
			l.setText(s);
		});
	}
	public <T> void updateArray(String message,T[] arr,BiConsumer<T,String> consumer){
		String[] split = message.split(" ");
		int a;
		for(a=0;a+1<split.length; a+=2){
			int index = a>>1;
			index%=arr.length;
			consumer.accept(arr[index],split[a]+" "+split[a+1]);
		}
		for(a>>=1;a<arr.length; a++){
			consumer.accept(arr[a],"");
		}
	}
}

