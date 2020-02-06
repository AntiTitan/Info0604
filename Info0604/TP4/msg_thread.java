import java.lang.Thread;

public class msg_thread extends Thread {
    int num;
    String msg;
    public msg_thread(int nb, String phrase) {
        num = nb;
        msg = phrase;
    }

    public void run() {
        System.out.println(msg + num);
    }
}