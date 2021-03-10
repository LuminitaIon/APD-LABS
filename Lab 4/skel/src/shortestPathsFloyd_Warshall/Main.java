package shortestPathsFloyd_Warshall;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
/**
 * @author cristian.chilipirea
 *
 *
 * aceeasi metoda ca in exercitiul anterior
 */
public class Main {

	public static void main(String[] args) {
		int M = 9;
		int[][] graph = { { 0, 1, M, M, M },
				          { 1, 0, 1, M, M }, 
				          { M, 1, 0, 1, 1 }, 
				          { M, M, 1, 0, M },
				          { M, M, 1, M, 0 } };
		int[][] graph2 = graph.clone();
		
		// Parallelize me (You might want to keep the original code in order to compare)
		for (int k = 0; k < 5; k++) {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
				}
			}
		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(graph[i][j] + " ");
			}
			System.out.println();
		}

		final int cores = Runtime.getRuntime().availableProcessors();
		final List<Thread> threads = IntStream
				.range(0, cores)
				.mapToObj(threadName -> new Thread(() -> {
					final int threadId = Integer.parseInt(Thread.currentThread().getName());
					final int start = (int) (threadId * ((double) 5 / cores));
					final int end = Math.min((int) ((threadId + 1) * ((double) 5 / cores)), 5);

					for (int k = 0; k < 5; k++) {
						for (int i = start; i < end; i++) {
							for (int j = 0; j < 5; j++) {
								graph2[i][j] = Math.min(graph2[i][k] + graph2[k][j], graph2[i][j]);
							}
						}
					}}, String.valueOf(threadName)))
				.collect(Collectors.toList());

		for (final Thread thread : threads) {
			thread.start();
		}

		try {
			for (final Thread thread : threads) {
				thread.join();
			}
		} catch (final InterruptedException interruptedException) {
			interruptedException.printStackTrace();
		}

		System.out.println();
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				System.out.print(graph2[i][j] + " ");
			}
			System.out.println();
		}
	}
}
