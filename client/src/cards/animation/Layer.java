package cards.animation;
import java.awt.image.BufferedImage;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.util.List;
import java.util.ArrayList;
public class Layer{
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
		return new Layer(buf,x,y);
	}

	private List<BufferedImage> buf = new ArrayList<BufferedImage>();
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
	private double _sinr,_cosr,_pax,_pay;

	public Layer(){
		this(null);
	}
	public Layer(BufferedImage buf){
		this(buf,0,0);
	}
	public Layer(BufferedImage buf,int x,int y){
		this.buf.add(buf);
		this.x=x;
		this.y=y;
		updateRatio();
	}
	public void addImage(BufferedImage buf){
		this.buf.add(buf);
	}
	public void setImage(int image){
		if(image < buf.size()){
			image_position = image;
			updateRatio();
		}
	}
	public BufferedImage getImage(int index){
		return buf.get(index);
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
		this._sinr = Math.sin(rotate);
		this._cosr = Math.cos(rotate);
		updatePaint();
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
		setPosition(x,this.y);
	}
	public final void setY(int y){
		setPosition(this.y,x);
	}
	public final void setPosition(int x, int y){
		this.x=x;
		this.y=y;
		updateCenter();
	}
	private final void updateRatio(){
		this._pax = -buf.get(image_position).getWidth()*px;
		this._pay = -buf.get(image_position).getHeight()*py;
		updatePaint();
	}
	private final void updatePaint(){
		this.paint_x = (int)(_pax*_cosr + _pay*_sinr);
		this.paint_y = (int)(_pay*_cosr + _pax*_sinr);
		updateCenter();
	}
	private final void updateCenter(){
		this.center_x = x-paint_x;
		this.center_y = y-paint_y;
	}
	public void paint(Graphics2D g){
		AffineTransform aft = g.getTransform();
		g.translate(center_x,center_y);
		g.rotate(rotate);
		if(buf!=null){
			g.drawImage(buf.get(image_position),paint_x,paint_y,null);
		}
		g.setTransform(aft);
	}
}
