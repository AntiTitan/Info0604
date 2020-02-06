#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include "ncurses.h"

#define NB_L	9
#define NB_C	9
#define p       3 
#define nb      NB_L/p   /*Nombres d'éléments contigus calculé par un thread*/
#define X       3

#define LARGEUR   NB_C*4+10               /* Largeur de la fenêtre */
#define HAUTEUR   NB_L*3+10              /* Hauteur de la fenêtre */
#define POSX      0                    /* Position horizontale de la fenêtre */
#define POSY      3                    /* Position verticale de la fenêtre */

typedef struct int_protege {
	pthread_mutex_t mutex;
	int compteur;
}int_p;

pthread_cond_t condAffich = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexCompteur = PTHREAD_MUTEX_INITIALIZER;
int_p cpt = {PTHREAD_MUTEX_INITIALIZER, 0};

int **matrice;
int *vecteur;
int *vecResultat;

WINDOW* creerFenetre(int h, int l, int y, int x) {
    WINDOW* fenetre;
    fenetre = newwin(h, l, y, x);
    box(fenetre,0,0);
    return fenetre;
}

void* GestionAffich(void* arg) {
    int statut;

    while(1) {
        statut = pthread_mutex_lock(&mutexCompteur);
        /*variable bloquée*/
		
		if ((cpt.compteur%X == 0)||(cpt.compteur == NB_C)) {
			statut = pthread_cond_broadcast(&condAffich);
		}

        /*variable débloquée*/
        statut = pthread_mutex_unlock(&mutexCompteur);
        sleep(5);
    }

    return NULL;
}

void* calculRes(void* arg) {
    int i, j, statut;
    int indLigne = *(int*)arg;

    for(j=indLigne; j<indLigne+nb; j++) {
        vecResultat[j] = 0;
        for (i=0; i<NB_C; i++) {
            vecResultat[j] = vecResultat[j] + matrice[j][i] * vecteur[i];
			
        }
		statut = pthread_mutex_lock(&mutexCompteur);
		cpt.compteur++;
		statut = pthread_mutex_unlock(&mutexCompteur);
		sleep(5);
    }
    return NULL;
}

void* affichage(void* args) {
	int i, a, statut;
	WINDOW* win = (WINDOW*)args;
	statut = pthread_mutex_lock(&mutexCompteur);
	while ((cpt.compteur%X !=0)&&(cpt.compteur!=NB_C)) {
		statut = pthread_cond_wait(&condAffich, &mutexCompteur);
	}
	for (i = 0; i < NB_L; i++) {
		a = i+2;
		afficherVecteur(vecResultat, NB_C, NB_L+15, win);
		wrefresh(win);
	}
	statut = pthread_mutex_unlock(&mutexCompteur);
}

void afficherMatrice(int** matrice, int m, int n, int pos, WINDOW* win) {
	int i, j, a, b;
	int x;
	mvwprintw(win, pos-1, 2, "**Matrice**\n");
	for (i = 0; i < m; i++){
		for (j = 0; j < n; j++){
			x=matrice[i][j];
			a=pos+1+i;
			b=2+2*j;
			mvwprintw(win, a, b, "%d", x);
		}
	}
	a=pos+2+m;
	mvwprintw(win, a, 2, "**Fin Matrice**\n");
	wrefresh(win);
}

void afficherVecteur(int* vecteur, int taille, int pos, WINDOW* win) {
	int i, a, b;
	int x;
	mvwprintw(win, pos, 2, "**Vecteur**\n");
	for (i = 0; i < taille; i++){
		x=vecteur[i];
		a=pos+2;
		b=3+4*i;
		mvwprintw(win, a, b,"%d", x);
	}
	a=pos+4;
	mvwprintw(win, a, 2, "**Fin Vecteur**\n");
	wrefresh(win);
}

int main(int argc, char *argv[]) {
    int* indLigne;
	int i, j, statut, k;
    void* res;
    pthread_t* threads;
	pthread_t gestionnaire;
	pthread_t affich;
	WINDOW* fenetre; 

	ncurses_initialiser();
	ncurses_couleurs();
	ncurses_souris();

	/* Vérification des dimensions du terminal */
	if((COLS < POSX + LARGEUR) || (LINES < POSY + HAUTEUR)) {
		ncurses_stopper();
		fprintf(stderr, 
			"Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n", 
			COLS, LINES, POSX + LARGEUR, POSY + HAUTEUR);
		exit(EXIT_FAILURE);
	}  

	/* Création de la fenêtre*/
	fenetre = creerFenetre(HAUTEUR, LARGEUR, POSY, POSX);
	/* Definition de la palette */
    init_pair(1, COLOR_BLACK, COLOR_RED);
	wbkgd(fenetre, COLOR_PAIR(1));
	wrefresh(fenetre);

	threads = malloc(sizeof(pthread_t)*p);

	srand(time(NULL));
	/*Initialisation matrice et vecteurs*/
	matrice = (int **) malloc(sizeof(int *) * NB_L);
	vecteur = (int *) malloc(sizeof(int) * NB_C);
	for (i = 0; i < NB_L; i++) {
		matrice[i] = (int *) malloc(sizeof(int) * NB_C);
		for (j = 0; j < NB_C; j++)
			matrice[i][j] = (rand() % 5) + 1;
	}
	for (i = 0; i < NB_C; i++) {
		vecteur[i] = (rand() % 5) + 1;
	}
	vecResultat = (int *) malloc(sizeof(int) * NB_L);
	for (i = 0; i < NB_L; i++) {
		vecResultat[i] = -1;	
	}

	/*Création des threads*/
	for (i = 0; i < p; i++)	{
        indLigne = (int*)malloc(sizeof(int));
        *indLigne = i*nb;
		statut = pthread_create(&threads[i], NULL, calculRes, indLigne);
	}
	statut = pthread_create(&affich, NULL, GestionAffich, NULL);
	/*Affichage de la matrice et des vecteurs*/
	printw("Pressez F2 pour quitter...\n");
	while((k = getch()) != KEY_F(2)) {
		mvwprintw(fenetre, 1, 2, "Fenetre d'affichage");
		afficherMatrice(matrice, NB_L, NB_C, 5, fenetre);
		afficherVecteur(vecteur, NB_C, NB_L+9, fenetre);
		statut = pthread_create(&affich, NULL, affichage, fenetre);
		wrefresh(fenetre);
	}

	/*Attente des threads*/
    for(i=0; i<p; i++) {
        statut = pthread_join(threads[i], &res);
    }
	pthread_join(gestionnaire,&res);
	pthread_join(affich,&res);

	/*Suppression de la fenetre et arret de ncurses*/
	delwin(fenetre);
	ncurses_stopper();
 
	/*Libération des ressources mémoires*/
	free(vecteur);
	free(vecResultat);
	for (i = 0; i < NB_L; i++)
		free(matrice[i]);
	free(matrice);
    free(threads);

	return EXIT_SUCCESS;
}
