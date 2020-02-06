import java.lang.Thread;

public class Exo1_Runnable {
    public static void main(String[] args) {
        int nb_thread;
        Runnable thread;
        nb_thread = Integer.parseInt(args[0]);
        for(int i=0; i<nb_thread; i++) {
            thread = new msg_runnable(i+1);
            new Thread(thread).start();
        }
    }
}

/*
http://www.enseignement.polytechnique.fr/informatique/profs/Eric.Goubault/PARA02/poly003.html
https://www.jmdoudoux.fr/java/dej/chap-threads.htm
*/