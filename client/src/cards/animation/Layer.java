package cards.animation;
import java.awt.image.BufferedImage;
import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.geom.AffineTransform;
import java.util.List;
import java.util.ArrayList;
public class Layer{
	private static int instances = 0;
	private int instance = instances++;
	public static Layer mergeLayers(int width, int height, Layer... layers){
		return mergeLayers(0,0,width,height,layers);
	}
	public static Layer mergeLayers(int x, int y, int width, int height, Layer... layers){
		BufferedImage buf = new BufferedImage(width,height,BufferedImage.TYPE_INT_ARGB);
		Graphics2D g2d = buf.createGraphics();
		g2d.translate(-x,-y);
		for(Layer layer: layers){
			layer.paint(g2d);
		}
		g2d.dispose();
		return new Layer(buf,x,y,layers[0].name+"-"+layers[layers.length-1].name);
	}

	private List<BufferedImage> buf = new ArrayList<BufferedImage>();
	private final String name;
	private int x;
	private int y;
	private double px = 0.5;
	private double py = 0.5;
	private double rotate = 0;
	private int image_position = 0;
	//precalculated
	private int center_x;
	private int center_y;
	private int paint_x;
	private int paint_y;
	private double zoom = 1;
	private boolean fromCenter = false;
	public Layer(){
		this(null);
	}
	public Layer(BufferedImage buf){
		this(buf,"Unnamed layer");
	}
	public Layer(BufferedImage buf,String name){
		this(buf,0,0,name);
	}
	public void swap(Layer layer){
		int tmp_x = x;
		int tmp_y = y;
		boolean tmp_fromCenter = fromCenter;
		setPosition(layer.x,layer.y,layer.fromCenter);
		layer.setPosition(tmp_x,tmp_y,tmp_fromCenter);
		double tmp_rotate = rotate;
		setRotation(layer.rotate);
		layer.setRotation(tmp_rotate);
		double tmp_center_x = center_x;
		double tmp_center_y = center_y;
		setCenter(layer.center_x,layer.center_y);
		layer.setCenter(tmp_center_x,tmp_center_y);
		double tmp_zoom = zoom;
		setZoom(layer.zoom);
		layer.setZoom(tmp_zoom);
	}
	public Layer(BufferedImage buf,int x,int y,String name){
		this.name = name;
		this.x=x;
		this.y=y;
		if(buf != null){
			this.buf.add(buf);
			updateRatio();
		}
	}
	public void addImage(BufferedImage buf){
		this.buf.add(buf);
	}
	public void setImageIndex(int image){
		if(image < buf.size()){
			image_position = image;
			updateRatio();
		}
	}
	public int getImageIndex(){
		return image_position;
	}
	public BufferedImage getImage(){
		return getImage(image_position);
	}
	public BufferedImage getImage(int index){
		return buf.get(index);
	}
	public int getNumberOfImages(){
		return buf.size();
	}
	public int getX(){
		return x;
	}
	public int getY(){
		return y;
	}
	public double getRotation(){
		return rotate;
	}
	public final void setRotation(double rotate){
		this.rotate = rotate;
	}
	public final void setCenterX(double px){
		setCenter(px,this.py);
	}
	public final void setCenterY(double py){
		setCenter(this.py,px);
	}
	public final void setCenter(double px, double py){
		this.px=px;
		this.py=py;
		updateRatio();
	}
	public final void setX(int x){
		setPosition(x,this.y,this.fromCenter);
	}
	public final void setY(int y){
		setPosition(this.x,y,this.fromCenter);
	}
	public void setZoom(double zoom){
		this.zoom = zoom;
	}
	public double getZoom(){
		return zoom;
	}
	public final void setPosition(int x, int y, boolean fromCenter){
		this.x=x;
		this.y=y;
		this.fromCenter = fromCenter;
		updateCenter();
	}
	private final void updateRatio(){
		if(buf.get(image_position) != null){
			this.paint_x = (int)(-buf.get(image_position).getWidth()*px);
			this.paint_y = (int)(-buf.get(image_position).getHeight()*py);
		}
		updateCenter();
	}
	private final void updateCenter(){
		this.center_x = x-paint_x;
		this.center_y = y-paint_y;
	}
	public void paint(Graphics2D g){
		AffineTransform aft = g.getTransform();
		if(fromCenter){
			g.translate(x,y);
		}else{
			g.translate(center_x,center_y);
		}
		g.rotate(rotate);
		g.scale(zoom,zoom);
		if(buf!=null){
			g.drawImage(buf.get(image_position),paint_x,paint_y,null);
		}
		g.setColor(Color.RED); //TODO debug remove
		g.drawLine(-5,-5,5,5); //TODO debug remove
		g.drawLine(-5,5,5,-5); //TODO debug remove
		g.setTransform(aft);
	}
	public String toString(){
		return "Layer: ["+x+";"+y+"] \""+name+"\"";
	}
}
