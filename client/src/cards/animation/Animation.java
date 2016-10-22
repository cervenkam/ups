package cards.animation;
import java.awt.Component;
import java.util.LinkedList;
import java.util.List;
import java.util.ArrayList;
import java.util.function.DoubleConsumer;
import java.util.function.DoubleSupplier;
import java.util.function.DoubleUnaryOperator;
public class Animation extends Thread{
	private static int running_threads = 0;
	private static Thread repaint_manager = null;
	private Layer layer;
	private final Component component;
	private LinkedList<List<Callback>> callbacks;
	public Animation(Layer layer,Component component){
		this.layer=layer;
		this.component=component;
		callbacks = new LinkedList<List<Callback>>();
	}
	public void setLayer(Layer layer){
		this.layer = layer;
	}
	public Layer getLayer(){
		return layer;
	}
	public Component getComponent(){
		return component;
	}
	public Animation andThen(double end,DoubleConsumer consumer,DoubleSupplier supplier){
		return andThen(new Callback(end,consumer,supplier));
	}
	public Animation andThen(double end,DoubleConsumer consumer,DoubleSupplier supplier,DoubleUnaryOperator operator){
		return andThen(new Callback(end,consumer,supplier,operator));
	}
	public Animation andThen(Callback callback){
		List<Callback> list = new ArrayList<Callback>();
		list.add(callback);
		callbacks.offerLast(list);
		return this;
	}
	public Animation parallel(double end,DoubleConsumer consumer,DoubleSupplier supplier){
		return parallel(new Callback(end,consumer,supplier));
	}
	public Animation parallel(double end,DoubleConsumer consumer,DoubleSupplier supplier,DoubleUnaryOperator operator){
		return parallel(new Callback(end,consumer,supplier,operator));
	}
	public Animation parallel(Callback callback){
		callbacks.peekLast().add(callback);
		return this;
	}
	public void run(){
		while(!callbacks.isEmpty()){
			List<Callback> list_of_callbacks = callbacks.remove();	
			List<Thread> list_of_threads = new ArrayList<Thread>();
			int min_sleep = Integer.MAX_VALUE;
			for(Callback callback: list_of_callbacks){
				if(callback.sleep < 0){
					min_sleep = 10;
				}else if(callback.sleep < min_sleep){
					min_sleep = callback.sleep;
				}
				Thread thr = new Thread(()->{
					long start_time = System.currentTimeMillis();
					long end_time = start_time + callback.duration;
					long curr_time;
					double start;
					synchronized(this){
						start = callback.supplier.getAsDouble();
					}
					while((curr_time = System.currentTimeMillis()) < end_time){
						double ratio = (double)(curr_time-start_time)/(end_time-start_time);
						synchronized(this){
							ratio = callback.operator.applyAsDouble(ratio);
							//System.out.println(getName()+": <"+(int)start+";"+(int)callback.end+">: "+(start+ratio*(callback.end-start)));
							callback.consumer.accept(start+ratio*(callback.end-start));
						}
						try{
							sleep(callback.sleep);
						}catch(InterruptedException e){}
					}
					synchronized(this){
						running_threads--;
					}
				});
				thr.start();
				running_threads++;
				list_of_threads.add(thr);
			}
			final int final_min_sleep = min_sleep>>1;
			if(repaint_manager == null){
				repaint_manager = new Thread(()->{
					while(running_threads>0){
						component.repaint();		
						try{
							sleep(final_min_sleep);
						}catch(InterruptedException e){}
					}
					repaint_manager=null;
				});
				repaint_manager.start();
			}
			for(Thread thread: list_of_threads){
				try{
					thread.join();
				}catch(InterruptedException e){}
			}
			component.repaint();		
		}
	}
}
