#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define SPIN 1000000000

typedef struct int_protege {
	pthread_mutex_t mutex;
	int compteur;
}int_p;

pthread_mutex_t mutexCompteur = PTHREAD_MUTEX_INITIALIZER;
int_p compteur = {PTHREAD_MUTEX_INITIALIZER, 0};

void* routineThread(void* arg) {
	int statut;

	statut = pthread_mutex_lock(&mutexCompteur);
	for(int i = 0; i < SPIN/4; i++)
		compteur.compteur++;
	statut = pthread_mutex_unlock(&mutexCompteur);
	return NULL;
}

int main(int argc, char *argv[]) {
	int i, diffTemps, statut;
    void* res;
	time_t temps1, temps2;
	pthread_t* thread;
	thread = malloc(sizeof(pthread_t)*4);

	temps1 = time(NULL);
	
	for (i=0; i<4; i++) {
		statut = pthread_create(&thread[i], NULL, routineThread, NULL);
	}
	for (i=0; i<4; i++) {
        pthread_join(thread[i],&res);
    }

	temps2 = time(NULL);
	diffTemps = (int) temps2 - (int) temps1;
	printf("Valeur finale du compteur : %d\n", compteur.compteur);
	printf("Temps : %d secondes\n", diffTemps);

	for(int i=0;i<4;i++){
        free(thread[i]);
    }
	free(thread);
	
	return 0;
}
