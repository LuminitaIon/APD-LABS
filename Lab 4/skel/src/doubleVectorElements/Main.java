package doubleVectorElements;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
/**
 * @author cristian.chilipirea
 *
 */
public class Main {

	public static void main(String[] args) {
		int N = 100000013;
		int v[] = new int[N];
		
		for(int i=0;i<N;i++)
			v[i]=i;
		
//		// Parallelize me
//		for (int i = 0; i < N; i++) {
//			v[i] = v[i] * 2;
//		}
		final int cores = Runtime.getRuntime().availableProcessors();
		final List<Thread> threads = IntStream.range(0, cores).mapToObj(threadName -> new Thread(() -> {
			final int threadId = Integer.parseInt(Thread.currentThread().getName());
			final int start = (int) (threadId * ((double) N / cores));
			final int end = Math.min((int) ((threadId + 1) * ((double) N / cores)), N);

			for (int i = start; i < end; i++) {
				v[i] = v[i] * 2;
			}}, String.valueOf(threadName)))
				.collect(Collectors.toList());
		for(final Thread thread : threads) {
			thread.start();
		}

		try {
			for(final Thread thread : threads) {
				thread.join();
			}
		} catch (final InterruptedException interruptedException) {
			interruptedException.printStackTrace();
		}

		for (int i = 0; i < N; i++) {
			if(v[i]!= i*2) {
				System.out.println("Wrong answer");
				System.exit(1);
			}
		}
		System.out.println("Correct");
	}

}
