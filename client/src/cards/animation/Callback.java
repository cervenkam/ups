package cards.animation;
import java.util.function.DoubleConsumer;
import java.util.function.DoubleSupplier;
import java.util.function.DoubleUnaryOperator;
public class Callback{
	public static final DoubleUnaryOperator LINEAR = (x)->x;
	public final DoubleConsumer consumer;
	public final DoubleSupplier supplier;
	public final DoubleUnaryOperator operator = LINEAR;
	public final int duration;
	public final int sleep;
	public final double end;
	public Callback(double end,DoubleConsumer consumer,DoubleSupplier supplier){
		this(end,consumer,supplier,2000);
	}
	public Callback(double end,DoubleConsumer consumer,DoubleSupplier supplier,int duration){
		this(end,consumer,supplier,duration,10);
	}
	public Callback(double end,DoubleConsumer consumer,DoubleSupplier supplier,int duration,int sleep){
		this.end=end;
		this.consumer=consumer;
		this.supplier=supplier;
		this.duration=duration;
		this.sleep=sleep;
	}
	public String toString(){
		return "Callback: "+supplier.getAsDouble()+"->"+end;
	}
}
