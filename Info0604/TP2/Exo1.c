#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef struct int_protege {
	pthread_mutex_t mutex;
	int compteur;
}int_p;

pthread_mutex_t mutexCompteur = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
int_p compteur = {PTHREAD_MUTEX_INITIALIZER, 0};

void* routineThread1(void* arg) {
	int statut;

    while(1) {
        statut = pthread_mutex_lock(&mutexCompteur);
        //variable bloquée

        compteur.compteur++;
        //printf("Je suis le thread incrémenteur      %d\n", compteur.compteur);
        if (compteur.compteur%2 == 0) pthread_cond_broadcast(&cond2);
        if (compteur.compteur >= 9) pthread_cond_broadcast(&cond3);
        
        //variable débloquée
        statut = pthread_mutex_unlock(&mutexCompteur);
        sleep(2);
    }

	return NULL;
}

void* routineThread2(void* arg) {
	int statut;
    int i = *(int*) arg;

    while(1) {
        //printf("Je suis le thread afficheur %d\n", i);
        statut = pthread_mutex_lock(&mutexCompteur);
        //variable bloquée
        while(compteur.compteur%2 != 0) {
            pthread_cond_wait(&cond2, &mutexCompteur);
        }
        printf("Thread%d Compteur: %d \n", i, compteur.compteur);
        //variable débloquée
        statut = pthread_mutex_unlock(&mutexCompteur);
        sleep(1);
    }

	return NULL;
}

void* routineThread3(void* arg) {
	int statut;
    int i = *(int*) arg;

    while(1) {
        //printf("Je suis le thread de remise à zéro %d\n", i-4);
        statut = pthread_mutex_lock(&mutexCompteur);
        //variable bloquée
        while(compteur.compteur < 9) {
            pthread_cond_wait(&cond3, &mutexCompteur);
        } // Attente de la condition sur le compteur
        compteur.compteur = 0;
        printf("compteur remis à zéro par thread %d\n", i-4);
        //variable débloquée
        statut = pthread_mutex_unlock(&mutexCompteur);
    }

	return NULL;
}


int main(int argc, char *argv[]) {
    int* indThread;
	int i, statut;
    void* res;
	pthread_t* thread;
	thread = malloc(sizeof(pthread_t)*9);
    /*
    while(1) {
        statut = pthread_create(&thread[1], NULL, routineThread1, NULL);
        statut = pthread_create(&thread[2], NULL, routineThread2, NULL);
        statut = pthread_create(&thread[3], NULL, routineThread3, NULL);
    }*/

    statut = pthread_create(&thread[0], NULL, routineThread1, NULL);
    for (i=1; i<5; i++) {
        indThread = (int*)malloc(sizeof(int));
        *indThread = i;
        statut = pthread_create(&thread[i], NULL, routineThread2, indThread);
    }
    for (i=5; i<9; i++) {
        indThread = (int*)malloc(sizeof(int));
        *indThread = i;
        statut = pthread_create(&thread[i], NULL, routineThread3, indThread);
    }
    
    for (i=0; i<9; i++) {
        pthread_join(thread[i],&res);
    }

    
	free(thread);
	
	return 0;
}
