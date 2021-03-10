package philosophersProblem;

/**
 * @author cristian.chilipirea
 * 
 */
public class Philosopher implements Runnable {
	Object leftFork, rightFork;
	int id;

	public Philosopher(int id, Object leftFork, Object rightFork) {
		this.leftFork = leftFork;
		this.rightFork = rightFork;
		this.id = id;
	}

	private void sleep() {
		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		// cand id-ul filosofului este 1 ia mai intai furculita din stanga
		// si apoi pe cea din dreapta
		if(id == 1) {
			synchronized (leftFork) {
				sleep(); // delay added to make sure the dead-lock is visible
				synchronized (rightFork) {
					System.out.println("Philosopher " + id + " is eating");
				}
			}
			// altfel o ia pe cea din dreapta si apoi pe cea din stanga
		} else {
			synchronized (rightFork) {
				sleep();
				synchronized (leftFork) {
					System.out.println("Philosopher" + id + " is eating");
				}
			}
		}
	}
}
