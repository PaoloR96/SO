#ifndef _lib
#define _lib

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 4

enum{LIBERO, OCCUPATO};

typedef struct{
	int stazione;
	
	pthread_mutex_t mutex;
	pthread_cond_t leggi;
	pthread_cond_t scrivi;
	int i;
	int num_lettori;
	int stato;
		
}monitor_treno;

void inizializza(monitor_treno*m);
void rimuovi(monitor_treno *m);

int leggi_stazione(monitor_treno *m);
void scrivi_stazione(monitor_treno *m, int stazione);

void *viaggiatori(void *p);
void *capo_treno(void * p);

#endif
