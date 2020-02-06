import java.lang.Thread;

public class Producteur extends Thread {
    BoiteMsg boiteP;
    String msg[];
    int taille;
    public Producteur(BoiteMsg boite, String[] messages) {
        boiteP = boite;
        taille = messages.length;
        msg = new String[taille];
        for (int i=0; i<taille; i++) {
            msg[i] = messages[i];
        }
    }

    public void run() {
        /*depose si possible puis dort entre 1000 et 5000 millisecondes*/
        int depose = 0, nb = 0;
        for (int i=0; i<taille; i++) {
            while(depose == 0) {
                depose = boiteP.deposer(msg[i]);
                try {
                    Thread.sleep(3000);
                }
                catch(InterruptedException e){
                    System.out.println("sleep error");
                }
            }
            depose = 0;
            nb = 1000 + (int)(Math.random() * 4001);
            try {
                Thread.sleep(nb);
            }
            catch(InterruptedException e){
                System.out.println("sleep error");
            }
        }
        while(depose == 0) {
            depose = boiteP.deposer("Termine");
        }
    }
}