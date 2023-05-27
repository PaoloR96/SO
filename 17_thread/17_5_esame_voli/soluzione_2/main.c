#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "thread.h"

//parametri passati al threads
typedef struct{
	int id;
	GestioneVoli *g;
}Param;

//procedura chiamata dai threads l'ho messa qui altrimenti la typedef Param la dovevo scrivere nel fil h
void *Aeroporto( void *param){
	Param *p = (Param*)param;
	int identificativo = p->id;
	GestioneVoli *g = p->g;

	InserisciVolo(g,identificativo );
	AggiornaVolo(g,identificativo ,1000);
	AggiornaVolo(g,identificativo ,2000);
	AggiornaVolo(g,identificativo ,1000);
	RimuoviVolo(g,identificativo );
	
	pthread_exit(NULL);
}

int main(){
	
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	int i;
	GestioneVoli *g;
	//genero i parametri per ogni tread
	Param p[NUM_THREADS];
	
	printf("\n\n __INIZIO__ \n\n");

	g = (GestioneVoli*)malloc(sizeof(GestioneVoli));
	g->dimensione = DIM;
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
		//salvo i paramentri da passare e li passo al thread
		p[i].id = i;
		p[i].g = g;
		pthread_create(&threads[i], &attr, Aeroporto, &p[i]);
	}
	
	for(i=0; i<NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	
	free(g);
	pthread_mutex_destroy(&(p->g->MUTEX));
	pthread_cond_destroy(&(p->g->SPAZIO_DISP));
	pthread_attr_destroy(&attr);
	
	printf("\n\n __FINE__ \n\n");
	
	pthread_exit(NULL);
}
