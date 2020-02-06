public class Exo2 {
    public static void main(String[] args) {
        BoiteMsg boite = new BoiteMsg();
        String msg[] = {"Voici mon premier produit", "Voila le second", "Et enfin le dernier"};
        new Consommateur(boite).start();
        new Producteur(boite, msg).start();
    }
}
/*
String[] s = new String[10];
BoiteMsg boite = new BoiteMsg();
Consommateur c;
Thread cons = new Thread(new Consommateur());
cons.start()
*/