#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "thread.h"

GestioneVoli *g;

int main(){
	
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	int i;
	
	printf("\n\n __INIZIO__ \n\n");

	g = (GestioneVoli*)malloc(sizeof(GestioneVoli));
	g->dimensione = 3;
	for(i=0; i<DIM; i++){
		g->vettore_stato[i] = LIBERO;
		g->vettore_voli[i].identificativo = 0;
		g->vettore_voli[i].quota = 0;
	}

	pthread_mutex_init(&(g->MUTEX),NULL);
	pthread_cond_init(&(g->SPAZIO_DISP),NULL);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(i=0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], &attr, Aeroporto, (void*)(long)i);
	}
	
	for(i=0; i<NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	
	free(g);
	pthread_mutex_destroy(&(g->MUTEX));
	pthread_cond_destroy(&(g->SPAZIO_DISP));
	pthread_attr_destroy(&attr);
	
	printf("\n\n __FINE__ \n\n");
	
	pthread_exit(NULL);
}
