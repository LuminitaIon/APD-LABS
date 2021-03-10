package helloWorld;

public class HelloWorldThread implements  Runnable{
    @Override
    public void run() {
        System.out.println("Hello from thread " + Thread.currentThread());
    }
}
