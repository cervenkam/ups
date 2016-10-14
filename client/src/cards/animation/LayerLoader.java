package cards.animation;
import java.util.List;
import java.util.ArrayList;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
public class LayerLoader{
	public static List<Layer> loadCards(int x,int y,String path){
		List<Layer> layers = loadLayers(path);
		BufferedImage second_side = loadImage(path+"/second_side.png");
		for(Layer layer:layers){
			layer.addImage(second_side);
			layer.setPosition(x,y);
			layer.setImage(1);
		}
		return layers;
	}
	public static List<Layer> loadLayers(String path){
		List<Layer> layers = new ArrayList<Layer>();
		File directory = new File(path);
		for(File file: directory.listFiles()){
			Layer layer = loadLayer(file);
			if(layer != null){
				layers.add(layer);
			}
		}
		return layers;
	}
	public static Layer loadLayer(String path){
		return loadLayer(new File(path));
	}
	private static Layer loadLayer(File file){
		return new Layer(loadImage(file));
	}
	private static BufferedImage loadImage(String path){
		return loadImage(new File(path));
	}
	private static BufferedImage loadImage(File file){
		BufferedImage buf;
		try{
			buf = ImageIO.read(file);
		}catch(IOException e){
			return null;
		}
		return buf;
	}
}
