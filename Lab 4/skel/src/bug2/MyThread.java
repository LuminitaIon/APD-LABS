package bug2;
/**
 * @author cristian.chilipirea
 * 
 *         Why does this code not block? We took the same lock twice!
 *         RASPUNS: putem face de 2 ori syncronized pe acelasi lucru
 *         prima data facem lock (syncronized) pe acel obicet (this)
 *         si a doua oara cand facem lock va chema acelasi obiect in
 *         executie, iar java permite acest lucru, deci executia va
 *         continua
 */
public class MyThread implements Runnable {
	static int i;

	@Override
	public void run() {
		synchronized (this) {
			synchronized (this) {
				i++;
			}
		}
	}
}
