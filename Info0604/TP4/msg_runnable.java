public class msg_runnable implements Runnable {
    int num;
    public msg_runnable(int nb) {
        num = nb;
    }

    public void run() {
        System.out.println("Hello from thread "+ num);
    }
}