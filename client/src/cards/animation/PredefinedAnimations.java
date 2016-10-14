package cards.animation;
import java.awt.Component;
public class PredefinedAnimations extends Animation{	
	private int x=-1;
	private int y=-1;
	private double rotate;
	private static final double ratio = 0.3;
	public PredefinedAnimations(Layer layer,Component component){
		super(layer,component);
		LayerManager.getInstance().split(layer);
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
	private void motion(){
		 andThen(x,(v)->setX(v),()->getX());
		parallel(y,(v)->setY(v),()->getY());
		parallel(rotate,(v)->setRotation(v),()->getRotation());
	}
	public boolean cardThrow(){
		if(x<0 || y<0){
			return false;
		}
		double tmp_rotate = rotate;
		double first_rotate = Math.PI;
		setRotation(getRotation()+first_rotate*ratio);
		setCenter(1.2,-0.6);
		andThen(rotate,(v)->setRotation(v),()->getRotation());
		setRotation(getRotation()+first_rotate*(1-ratio)+Math.PI*(Math.random()-0.5)/10);
		 andThen(x,(v)->setX(v),()->getX());
		parallel(y,(v)->setY(v),()->getY());
		parallel(rotate,(v)->setRotation(v),()->getRotation());
		rotate = tmp_rotate;
		run();
		return true;
	}
	public boolean getCard(){
		if(x<0 || y<0){
			return false;
		}
		 andThen(x,(v)->setX(v),()->getX());
		parallel(y,(v)->setY(v),()->getY());
		parallel(rotate,(v)->setRotation(v),()->getRotation());
		run();
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
	private int getPart(double start,double end){
		return (int)((ratio-start)/(end-start));
	}
}
