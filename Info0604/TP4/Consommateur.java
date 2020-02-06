import java.lang.Thread;

public class Consommateur extends Thread {
    BoiteMsg boiteC;
    public Consommateur(BoiteMsg boite) {
        boiteC = boite;
    }

    public void run() {
        /*recupere si possible, affiche puis dort entre 1000 et 5000 millisecondes tant qu'il ne récupere pas "Termine"*/
        int nb = 0, fin = 0;
        String msg = "";
        while(fin==0) {
            while (msg.equals("")==true) {
                msg = boiteC.recuperer();
                try {
                    Thread.sleep(3000);
                }
                catch(InterruptedException e){
                    System.out.println("sleep error");
                }
            }
            System.out.println(msg);
            msg = new String("");
            nb = 1000 + (int)(Math.random() * 4001);
            try {
                Thread.sleep(nb);
            }
            catch(InterruptedException e){
                System.out.println("sleep error");
            }
            
            if (msg.equals("Termine") == true) {
                System.out.println("equals");
                fin = 1;
            }
        }
    }
}