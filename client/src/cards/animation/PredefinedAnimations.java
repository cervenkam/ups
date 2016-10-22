package cards.animation;
import java.awt.Component;
public class PredefinedAnimations extends Animation{	
	private int x=-1;
	private int y=-1;
	private double rotate = 0;
	private double zoom = 1;
	private static final double ratio = 0.3;
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
	public boolean getCard(boolean visible){
		if(x<0 || y<0){
			return false;
		}
		LayerManager.getInstance().pushOnTop(getLayer());
		LayerManager.getInstance().split(getLayer());
		setMaxZoom(2);
		 andThen(x,(v)->setX(v),()->getX());
		parallel(y,(v)->setY(v),()->getY());
		parallel(rotate,(v)->setRotation(v),()->getRotation());
		parallel(zoom,(v)->setZoom(v),()->getZoom(),Callback.SINUS);
		System.out.println("Animating "+getLayer()+" to ["+x+";"+y+"]");
		run();
		getLayer().setImage(visible?0:1);
		cleanUp();
		return true;
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
