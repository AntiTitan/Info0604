#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NB_T   4            /*Nombre de threads*/
#define nb     5            /*Nombre de valeurs traitees par un thread*/
#define NB_V   NB_T*nb      /*Nombre de valeurs du tableau*/

typedef struct somme_val {
	pthread_mutex_t mutex;
	int somme; /*Nombre d'étrangers présents dans la salle*/
}somme_t;

int tab[NB_V];

void initTab(int tab[]) {
    int i;

    srand(time(NULL));
    for(i=0;i<NB_V;i++) {
        tab[i] = rand() % 50 + 1;
    }
}

pthread_mutex_t mutexSomme = PTHREAD_MUTEX_INITIALIZER;
somme_t somme = {PTHREAD_MUTEX_INITIALIZER, 0};


void* Addition(void* arg) {
    int statut, s=0, ind = *(int*) arg;

    for (int i=ind; i<ind+nb; i++) {
        statut = pthread_mutex_lock(&mutexSomme);
        //variable bloquée

        somme.somme += tab[i];
        s += tab[i];

        //variable débloquée
        statut = pthread_mutex_unlock(&mutexSomme);
    }
    printf("Thread %d somme tab[%d] a tab[%d]: %d\n", ind/nb, ind, ind+nb, s);

    return NULL;
}

int main(int argc, char *argv[]) {
    int* indThread;
	int i, statut;
    void* res;
	pthread_t* threads;
	threads = malloc(sizeof(pthread_t)*NB_T);

    initTab(tab);
    for (i=0; i<NB_V; i++) {
        printf("\t %d ", tab[i]);
    }
    printf("\n");

    for(i=0; i<NB_T; i++) {
        indThread = (int*)malloc(sizeof(int));
        *indThread = i*nb;
        statut = pthread_create(&threads[i], NULL, Addition, indThread);
    }

    for(i=0; i<NB_T; i++) {
        statut = pthread_join(threads[i], &res);
    }
    
    printf("Somme des valeurs du tableau: %d \n", somme.somme);
    
	free(threads);
	
	return 0;
}