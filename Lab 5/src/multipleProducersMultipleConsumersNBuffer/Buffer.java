package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;

/**
 * @author Gabriel Gutu <gabriel.gutu at upb.ro>
 *
 */
public class Buffer {
    
    Queue queue;
    int size;
    int Currsize;
    
    public Buffer(int size) {
        queue = new LimitedQueue(size);
        this.size = size;
        this.Currsize = 0;
    }
    // produce si pune in buffer
    void produce(int value) {
        queue.add(value);
        Currsize++;
    }
    // consuma si pune in buffer
    void consume() {
        Currsize--;
    }
    // daca buffer-ul e plin, atunci marimea curenta a buffer-ului
    // e egala cu marimea lui maxima
    boolean isFull() {
        return Currsize == size;
    }
	void put(int value) {
        synchronized (this) {
            while (isFull()) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            produce(value);
            notify();
        }
	}

	int get() {
        synchronized (this) {
            while (!isFull()) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            consume();
            notify();
            return (int)queue.poll();
        }
	}
}
