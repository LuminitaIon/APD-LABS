package multipleProducersMultipleConsumers;
// aici modificam

import java.util.concurrent.ArrayBlockingQueue;


public class Buffer {
	private static final int Cap = 1; // capacitate

	private final ArrayBlockingQueue<Integer> queue;

	public Buffer() {
		this.queue = new ArrayBlockingQueue<>(Cap);
	}
	void put(final int value) {
		try {
			this.queue.put(value);
		} catch (final InterruptedException e) {
			e.printStackTrace();
		}
	}

	int get() {
		try {
			return this.queue.take();
		} catch (final InterruptedException | NullPointerException e) {
			e.printStackTrace();
			return 0;
		}
	}
}
