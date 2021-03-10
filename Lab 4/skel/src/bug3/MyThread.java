package bug3;
/**
 * @author cristian.chilipirea
 * 
 *         Why is value set correct even though we use different locks in
 *         different threads?
 *
 *         RASPUNS: Obiectele a si b sunt, de fapt acelasi obiect si amandoua
 *         pointeaza la "LOCK"
 */
public class MyThread implements Runnable {
	static String a = "LOCK";
	/*aici punem alta referinta sau punem new String("LOCK");
	si facem alt obiect cu acelasi continut ca si inainte
	*/
	static String b = "LOCK2";
	int id;
	static int value = 0;

	MyThread(int id) {
		this.id = id;
	}

	@Override
	public void run() {
		if (id == 0) {
			synchronized (a) {
				for (int i = 0; i < Main.N; i++)
					value = value + 3;
			}
		} else {
			synchronized (b) {
				for (int i = 0; i < Main.N; i++)
					value = value + 3;
			}
		}
	}
}
