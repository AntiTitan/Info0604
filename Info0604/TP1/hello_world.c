#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void * msg_thread(void * arg){
    int * tab = (int *)arg;
    int k,nb_th,statut;
    k=tab[1];
    nb_th=tab[0];
    /*statut = pthread_detach(pthread_self());
    if(statut!=0){
        printf("PB dans le thread %d\n",k);
    }*/
    printf("Hello World ! du thread %d de %d\n",k,nb_th);
    fflush(stdout);
    return NULL;
}

int main(int argc, char* argv[]) {
    int nb_th,statut;
    int ** tab;
    void * res;
    pthread_t* thread;

    
    if (argc!=2) {
        printf("Nombre incorrect d'arguments, un nombre de thread est attendu\n");
        return EXIT_FAILURE;
    }
    nb_th = atoi(argv[1]);
    thread = malloc(sizeof(pthread_t)*nb_th);
    tab = malloc(sizeof(int *)*nb_th);
    for(int i=0;i<nb_th;i++){
        tab[i]=malloc(sizeof(int)*2);
        tab[i][0]=nb_th;
    }
    for (int i=0;i<nb_th;i++){
        tab[i][1]=i+1;
        statut = pthread_create(&thread[i],NULL,msg_thread,tab[i]);
        if(statut!=0){
            printf("Pb création thread\n");
        }
    }
    for (int i=0;i<nb_th;i++) {
        pthread_join(thread[i],&res);
    }
    free(thread);
    for(int i=0;i<nb_th;i++){
        free(tab[i]);
    }
    free(tab);

    return 0;
}