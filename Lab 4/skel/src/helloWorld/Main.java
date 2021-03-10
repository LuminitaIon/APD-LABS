package helloWorld;

public class Main {
    public static void main(String[] args) {
        final int cores = Runtime.getRuntime().availableProcessors();

        final Thread[] threads = new Thread[cores];
        for(int i = 0; i < cores; i++) {
            threads[i] = new Thread(new HelloWorldThread());
            threads[i].start();
        }
        try {
            for(final Thread thread : threads) {
                thread.join();
            }
        } catch (final InterruptedException interruptedException) {
            interruptedException.printStackTrace();
        }
    }
}
