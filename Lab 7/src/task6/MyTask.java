package task6;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.atomic.AtomicInteger;

public class MyTask extends RecursiveTask<Void> {

    int[] graph;
    int step;

    public MyTask(int[] graph, int step) {
        this.graph = graph;
        this.step= step;
    }

    @Override
    public Void compute() {
        if (Main.N == step) {
            printQueens(graph);
            return null;
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            List<MyTask> tasks = new ArrayList<>();

            if (check(newGraph, step)) {

                MyTask t = new MyTask(newGraph, step + 1);
                tasks.add(t);
                t.fork();
            }
            for (MyTask task: tasks) {
                task.join();
            }
        }
        return null;
    }

    private static boolean check(int[] arr, int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    private static void printQueens(int[] sol) {
        StringBuilder aux = new StringBuilder();
        for (int i = 0; i < sol.length; i++) {
            aux.append("(").append(sol[i] + 1).append(", ").append(i + 1).append("), ");
        }
        aux = new StringBuilder(aux.substring(0, aux.length() - 2));
        System.out.println("[" + aux + "]");
    }
}