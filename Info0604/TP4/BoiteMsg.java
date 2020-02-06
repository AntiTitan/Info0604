public class BoiteMsg {
    int contient;
    String msgBoite;

    public BoiteMsg(){
        contient = 0;
        msgBoite = "";
    }

    public int deposer(String msg) {
        if (contient == 0) {
            msgBoite = new String(msg);
            contient = 1;
            System.out.println("Message depose dans la boite");
            return 1;
        }
        else {
            System.out.println("La boite est pleine. Tentez plus tard.");
            return 0;
        }
    }

    public String recuperer() {
        String msg = "";
        if (contient == 1) {
            msg = new String(msgBoite);
            msgBoite = new String("");
            contient = 0;
            System.out.println("Message recupere dans la boite");
            return msg;
        }
        else {
            System.out.println("La boite est vide. Tentez plus tard.");
            return "";
        }
    }
}