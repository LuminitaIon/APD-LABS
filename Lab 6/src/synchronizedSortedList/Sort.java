package synchronizedSortedList;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Sort extends Thread {
    private final List<Integer> list;
    Semaphore S;

    public Sort(List<Integer> list, Semaphore S) {
        super();
        this.list = list;
        this.S = S;
    }

    @Override
    public void run() {
        try {
            S.acquire(3);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Collections.sort(list);
        S.release();
    }
}
