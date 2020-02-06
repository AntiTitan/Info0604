#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NB_G   4   /*Nombre de guichets*/
#define CAP    10  /*Capacité de la salle d'attente*/

typedef struct salle_attente {
	pthread_mutex_t mutex;
	int nbE; /*Nombre d'étrangers présents dans la salle*/
}salleA;

pthread_cond_t placeLibre = PTHREAD_COND_INITIALIZER;
pthread_cond_t etrangerPresent = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexSalle = PTHREAD_MUTEX_INITIALIZER;
salleA salle = {PTHREAD_MUTEX_INITIALIZER, 0};

void* GestionPref(void* arg) {
    int statut;

    while(1) {
        statut = pthread_mutex_lock(&mutexSalle);
        //variable bloquée
        printf("Gestion %d\n", salle.nbE);

        if (salle.nbE < 10) {
            pthread_cond_broadcast(&placeLibre);
        }

        if (salle.nbE >= 1) {
            pthread_cond_broadcast(&etrangerPresent);
        }

        //variable débloquée
        statut = pthread_mutex_unlock(&mutexSalle);
        sleep(5);
    }

    return NULL;
}

void* ArriveeEtranger(void* arg) {
	int statut, nbE, i;

    while(1) {
        statut = pthread_mutex_lock(&mutexSalle);
        //variable bloquée

        while(salle.nbE >= 10) {
            pthread_cond_wait(&placeLibre, &mutexSalle);
        }// Attente du signal indiquant qu'il y a des places libres dans la salle
        
        printf("Combien d'étrangers viennent d'arriver? ");
        scanf("%d", &nbE);

        //if (nbE>50) return EXIT_FAILURE;
        salle.nbE = salle.nbE + nbE;

        //variable débloquée
        statut = pthread_mutex_unlock(&mutexSalle);
        sleep(2);
    }

	return NULL;
}

void* PriseEnCharge(void* arg) {
    int statut, i = *(int*) arg;

    while(1) {
        statut = pthread_mutex_lock(&mutexSalle);
        //variable bloquée

        while(salle.nbE < 1) {
            pthread_cond_wait(&etrangerPresent, &mutexSalle);
        }// Attente du signal indiquant qu'il y a des etrangers dans la salle
        salle.nbE--;
        printf("Le prochain étranger est appelé au guichet %d, il reste %d personnes dans la salle d'attente.\n", i, salle.nbE);

        //variable débloquée
        statut = pthread_mutex_unlock(&mutexSalle);
        sleep(20); //Le temps de traiter le dossier de l'etranger servi au guichet i
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int* indThread;
	int i, statut;
    void* res;
    pthread_t gerant;
    pthread_t salleAttente;
	pthread_t* guichets;
	guichets = malloc(sizeof(pthread_t)*NB_G);

    statut = pthread_create(&gerant, NULL, GestionPref, NULL);
    statut = pthread_create(&salleAttente, NULL, ArriveeEtranger, NULL);
    for(i=0; i<NB_G; i++) {
        indThread = (int*)malloc(sizeof(int));
        *indThread = i;
        statut = pthread_create(&guichets[i], NULL, PriseEnCharge, indThread);
    }

    pthread_join(gerant, &res);
    pthread_join(salleAttente, &res);
    for(i=0; i<NB_G; i++) {
        statut = pthread_join(guichets[i], &res);
    }
    
    
	free(guichets);
	
	return 0;
}