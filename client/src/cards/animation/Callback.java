package cards.animation;
import java.util.function.DoubleConsumer;
import java.util.function.DoubleSupplier;
import java.util.function.DoubleUnaryOperator;
public class Callback{
	public static final int DURATION = 800;
	public static final int SLEEP = 10;
	public static final DoubleUnaryOperator LINEAR = (x)->x;
	public static final DoubleUnaryOperator SINUS = (x)->Math.sin(x*Math.PI);
	public static final DoubleUnaryOperator SINUS_HALF = (x)->Math.sin(x*Math.PI/2);
	public final DoubleConsumer consumer;
	public final DoubleSupplier supplier;
	public final DoubleUnaryOperator operator;
	public final int duration;
	public final int sleep;
	public final double end;
	public Callback(double end,DoubleConsumer consumer,DoubleSupplier supplier){
		this(end,consumer,supplier,DURATION);
	}
	public Callback(double end,DoubleConsumer consumer,DoubleSupplier supplier,DoubleUnaryOperator operator){
		this(end,consumer,supplier,DURATION,SLEEP,operator);
	}
	public Callback(double end,DoubleConsumer consumer,DoubleSupplier supplier,int duration){
		this(end,consumer,supplier,duration,SLEEP);
	}
	public Callback(double end,DoubleConsumer consumer,DoubleSupplier supplier,int duration,int sleep){
		this(end,consumer,supplier,duration,sleep,LINEAR);
	}
	public Callback(double end,DoubleConsumer consumer,DoubleSupplier supplier,int duration,int sleep,DoubleUnaryOperator operator){
		this.end=end;
		this.consumer=consumer;
		this.supplier=supplier;
		this.duration=duration;
		this.sleep=sleep;
		this.operator=operator;
	}
	public String toString(){
		return "Callback: "+supplier.getAsDouble()+"->"+end;
	}
}
