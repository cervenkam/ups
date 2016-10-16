package cards.animation;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.awt.Graphics2D;
import static java.awt.Color.*;
public class LayerManager{
	private static final int width = 800; //TODO
	private static final int height = 600; //TODO
	private static final LayerManager manager = new LayerManager();
	private List<Layer> layers;
	private List<Layer> merges;
	private List<Layer> cards;
	public static LayerManager getInstance(){
		return manager;
	}
	private LayerManager(){
		layers = new ArrayList<Layer>();
		merges = new ArrayList<Layer>();
		cards = new ArrayList<Layer>();
	}
	public void setLayers(List<Layer> layers){
		this.cards.clear();
		this.layers.clear();
		this.cards.addAll(layers);
		this.layers.addAll(layers);
	}
	public Layer getCard(int index){
		return cards.get(index);
	}
	public void addLayersOnTop(List<Layer> layers){
		this.layers.addAll(layers);
	}
	public void addLayerOnTop(Layer layer){
		layers.add(layer);
	}
	public void addLayerOnBottom(Layer layer){
		layers.add(0,layer);
	}
	public void addLayersOnBottom(List<Layer> layers){
		this.layers.addAll(0,layers);
	}
	public void pushOnTop(Layer layer){
		layers.add(layers.remove(layers.indexOf(layer)));
	}
	public void split(Layer... layers){
		int[] positions = new int[layers.length];
		for(int a=0; a<layers.length; a++){
			positions[a] = this.layers.indexOf(layers[a]);
		}
		split(positions);
	}
	public void mergeAll(){
		split(new int[0]);
	}
	private void split(int[] position){
		merges.clear();
		if(position == null){
			position = new int[0];
		}
		Arrays.sort(position);
		for(int a=0; a<=position.length; a++){
			int start = a == 0              ? 0              : position[a-1]+1;
			int end   = a == position.length? layers.size()-1: position[a]-1;
			Layer[] to_merge = new Layer[end-start+1];
			for(int b=start; b<=end; b++){
				to_merge[b-start] = layers.get(b);
			}
			if(to_merge.length > 0){
				merges.add(Layer.mergeLayers(width,height,to_merge));
			}
			if(a!=position.length){
				merges.add(layers.get(position[a]));
			}
		}
	}
	public void paint(Graphics2D g){
		g.setColor(WHITE);
		g.fillRect(0,0,width,height);
		System.out.println("Drawing: "+merges.size());
		for(int a=0; a<merges.size(); a++){
			merges.get(a).paint(g);
		}
	}
}
