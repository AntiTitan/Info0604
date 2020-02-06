public class Exo1_Thread {
    public static void main(String[] args) {
        int nb_thread;
        nb_thread = Integer.parseInt(args[0]);
        for(int i=0; i<nb_thread; i++) {
            new msg_thread(i+1,"Vous avez le bonjour du thread ").start();
        }
    }
}