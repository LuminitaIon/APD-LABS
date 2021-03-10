package bug4;
/**
 * @author cristian.chilipirea
 *
 *         Why doesn't this program end? (Hint: volatile)
 *         RASPUNS: Nu se termina pentru ca variabila keepRunning nu este
 *         sincronizata intre thread-uri. Thread-ul nu va sti cand sa se termine,
 *         deoarece variabila nu se modifica in memoria principala, deci o vom face
 *         de tip volatile
 */
public class Main extends Thread {
	/* facem aceasta variabila volatile
	*/
	volatile boolean keepRunning = true;

	public void run() {
		long count = 0;
		while (keepRunning) {
			count++;
		}

		System.out.println("Thread terminated." + count);
	}

	public static void main(String[] args) throws InterruptedException {
		Main t = new Main();
		t.start();
		Thread.sleep(1000);
		t.keepRunning = false;
		System.out.println("keepRunning set to false.");
	}
}
