package bug5;
import java.util.concurrent.BrokenBarrierException;
/**
 * @author cristian.chilipirea
 * 
 *         Solve the dead-lock
 */
public class MyThreadB implements Runnable {

	@Override
	public void run() {
		try {
			Main.barrier.await();
		} catch (final InterruptedException | BrokenBarrierException interruptedException) {
			interruptedException.printStackTrace();
		}
		synchronized (Main.lockB) {
			for (int i = 0; i < Main.N; i++)
				Main.valueB++;
			synchronized (Main.lockA) {
				for (int i = 0; i < Main.N; i++)
					Main.valueA++;
			}
		}
	}
}
