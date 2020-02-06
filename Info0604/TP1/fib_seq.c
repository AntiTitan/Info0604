#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

//Q1:fib(39)

unsigned int fib(int n) {
	unsigned int x, y;
	if (n <= 1)
		return n;
	else {
		x = fib(n - 1);
		y = fib(n - 2);
		return x + y;
	}
}

void* routineThread(void* arg) {
	unsigned int f;
	int n = *(int*)arg;
	f = fib(n);
	pthread_exit(f);
}

int main(int argc, char *argv[]) {
	unsigned int f,x,y;
	int n, diffTemps, statut, tmp1, tmp2;
	time_t temps1, temps2;
	pthread_t fibn1, fibn2;

	if (argc!=2) {
        printf("Nombre incorrect d'arguments, un nombre est attendu\n");
        return EXIT_FAILURE;
    }

	n = atoi(argv[1]);	
	temps1 = time(NULL);
	printf("The main \n");

	tmp1=n-1;
	tmp2=n-2;
	statut = pthread_create(&fibn1, NULL, routineThread, &tmp1);
	printf("pthread 1 created \n");
	statut = pthread_create(&fibn2, NULL, routineThread, &tmp2);
	printf("pthread 2 created \n");

    pthread_join(fibn1,&x);
    pthread_join(fibn2,&y);

	f = x+y;
	temps2 = time(NULL);
	diffTemps = (int) temps2 - (int) temps1;
	printf("Valeur du %d-ieme nombre de Fibonacci : %u\n", n, f);
	printf("Temps : %d secondes\n", diffTemps);
	
	return EXIT_SUCCESS;
}
