package task1;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;

public class MyThread extends Thread{
    private ExecutorService tpe;
    private final ArrayList<Integer> path;
    private final int dest;

    public MyThread(ExecutorService Tpe, ArrayList<Integer> Pt, int Dst) {
        tpe = Tpe;
        path = Pt;
        dest = Dst;
    }

    @Override
    public void run() {
        if(path.get(path.size() - 1) == dest) {
                System.out.println(path);
                tpe.shutdown();
        }
        int lastN = path.get(path.size() - 1);

        for(int i = 0; i < Main.graph.length; i++) {
            if(Main.graph[i][0] == lastN && !path.contains(Main.graph[i][1])) {
                ArrayList<Integer> newPath = (ArrayList<Integer>) path.clone();
                newPath.add(Main.graph[i][1]);
                tpe.submit(new MyThread(tpe, newPath, dest));
            }
        }
    }
}
