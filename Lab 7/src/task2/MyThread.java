package task2;
import java.util.concurrent.ExecutorService;

public class MyThread extends Thread {
    private ExecutorService tpe;
    private final int[] Clrs;
    private final int step;
    private int i;
    MyThread(ExecutorService tpe, int[] colors, int step) {
        this.tpe = tpe;
        Clrs = colors;
        this.step = step;
    }

    private void printC () {
        StringBuilder buff = new StringBuilder();

        for(int c : Clrs) {
            buff.append(c).append(" ");
        }
        System.out.println(buff);
    }

    private  boolean verifyC(int[] newC) {
        for(i = 0; i < step; i++) {
            if(newC[i] == newC[step] && isEdge(i, step)) {
                return false;
            }
        }
        return true;
    }

    private boolean isEdge(int a, int b) {
        for(i = 0; i < Main.graph.length; i++) {
            if(Main.graph[i][0] == a && Main.graph[i][1] == b)
                return true;
        }
        return false;
    }
    @Override
    public void run() {
        if(step == Main.N) {
            printC();
            tpe.shutdown();
        }

        for (i = 0; i < Main.COLORS; i++) {
            int[] newC = Clrs.clone();
            newC[step] = i;

            if(verifyC(newC)) {
                tpe.submit(new MyThread(tpe, newC, step+1));
            }
        }
    }
}
