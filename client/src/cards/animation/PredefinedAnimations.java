package cards.animation;
import java.awt.Component;
import java.util.Map;
import java.util.HashMap;
public class PredefinedAnimations extends Animation{	
	private int x=-1;
	private int y=-1;
	private double rotate = 0;
	private double zoom = 1;
	private static final double ratio = 0.3;
	private String curr_anim = "DontWantToContinue";
	private static final Map<String,Layer> animations = new HashMap<String,Layer>();
	static{
		LayerManager lm = LayerManager.getInstance();
		Layer dont_want_to_continue = LayerLoader.loadLayerImages("images/animations/DontWantToContinue");
		lm.addLayerOnBottom(dont_want_to_continue);
		animations.put("DontWantToContinue",dont_want_to_continue);
	}
	public PredefinedAnimations(Layer layer,Component component){
		super(layer,component);
	}
	public PredefinedAnimations setPosition(int x,int y){
		this.x=x;
		this.y=y;
		return this;
	}
	public PredefinedAnimations setRotate(double rotate){
		this.rotate = rotate;
		return this;
	}
	public void setMaxZoom(double zoom){
		this.zoom = zoom;
	}
	public void cleanUp(){
		LayerManager.getInstance().pushOnCorrectPosition(getLayer());
		getComponent().repaint();	
	}
	public boolean animation(String name){
		return animation(name,0);
	}
	public boolean animation(String name,double angle){
		if(x<0 || y<0){
			return false;
		}
		LayerManager lm = LayerManager.getInstance();
		setLayer(animations.get(name));
		getLayer().setRotation(angle);
		getLayer().setX(x);
		getLayer().setY(y);
		getLayer().setZoom(zoom);
		lm.pushOnTop(getLayer());
		lm.split(getLayer());
		int curr = getImageIndex();
		int count = getLayer().getNumberOfImages();
		andThen(count-1,(v)->setImageIndex(v),()->getImageIndex());
		lm.print();
		run();
		getLayer().setImageIndex(curr);
		lm.pushOnBottom(getLayer());
		getComponent().repaint();	
		return true;
	}
	public boolean getCard(boolean visible){
		if(x<0 || y<0){
			return false;
		}
		LayerManager lm = LayerManager.getInstance();
		lm.pushOnTop(getLayer());
		lm.split(getLayer());
		setMaxZoom(2);
		 andThen(x,(v)->setX(v),()->getX());
		parallel(y,(v)->setY(v),()->getY());
		parallel(rotate,(v)->setRotation(v),()->getRotation());
		parallel(zoom,(v)->setZoom(v),()->getZoom(),Callback.SINUS);
		System.out.println("Animating "+getLayer()+" to ["+x+";"+y+"]");
		lm.print();
		run();
		getLayer().setImageIndex(visible?0:1);
		cleanUp();
		return true;
	}
	private int getImageIndex(){
		return getLayer().getImageIndex();
	}
	private void setImageIndex(double image_index){
		getLayer().setImageIndex((int)image_index);
	}
	private int getX(){
		return getLayer().getX();
	}
	private int getY(){
		return getLayer().getY();
	}
	private double getRotation(){
		return getLayer().getRotation();
	}
	private double getZoom(){
		return getLayer().getZoom();
	}
	private void setCenter(double cx,double cy){
		getLayer().setCenter(cx,cy);
	}
	private void setX(double x){
		getLayer().setX((int)x);
	}
	private void setY(double y){
		getLayer().setY((int)y);
	}
	private void setRotation(double r){
		getLayer().setRotation(r);
	}
	private void setZoom(double zoom){
		getLayer().setZoom(zoom);
	}
	private int getPart(double start,double end){
		return (int)((ratio-start)/(end-start));
	}
}
