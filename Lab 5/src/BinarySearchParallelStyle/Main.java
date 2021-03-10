package BinarySearchParallelStyle;
import java.util.concurrent.CyclicBarrier;
public class Main {
    public static void main(String args[]) {
        int N = 20;
        int P = 5;
        int v[] = new int[N];
        int x = 13;
        int res;

        CyclicBarrier barrier = new CyclicBarrier(P);
        MyThread[] threads = new MyThread[N];

        for(int i = 0; i < N; i++) {
            v[i] = i ;
        }

        for(int i = 0; i < P; i++){
            threads[i] = new MyThread(N, P, v, i, x, barrier);
        }

        for(int i = 0; i < P; i++) {
            threads[i].start();
        }

        for(int i = 0; i < P; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        res = MyThread.getResult();

        if(res != -1) {
            System.out.println("Element found at position " + res);
        } else {
            System.out.println("Element not found");
        }
    }
}
