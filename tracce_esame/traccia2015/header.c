#include "header.h"
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h> // rand
#include <unistd.h> // sleep
#include <stdio.h> // printf

void inizializza(GestioneIO *gPtr){
	pthread_cond_init( &gPtr->NOT_FULL, NULL);
	pthread_cond_init( &gPtr->NOT_EMPTY, NULL);
	pthread_mutex_init( &gPtr->mutex, NULL);
}

void rimuovi(GestioneIO *gPtr){
	pthread_cond_destroy( &gPtr->NOT_FULL );
	pthread_cond_destroy( &gPtr->NOT_EMPTY );
	pthread_mutex_destroy( &gPtr->mutex );
}

void produci(GestioneIO *gPtr, Buffer *bPtr){
	while( gPtr->coda + 1 == gPtr->testa ) // buffer pieno
		pthread_cond_wait( &gPtr->NOT_FULL, &gPtr->mutex );

	pthread_mutex_lock( &gPtr->mutex );
	gPtr->vettore[gPtr->coda] = *bPtr;
	gPtr->coda = (gPtr->coda + 1) % DIM_BUFF;

	pthread_mutex_unlock( &gPtr->mutex );
}

int consuma(GestioneIO *gPtr, Buffer *bPtr){
	if( gPtr->testa == gPtr->coda ) // buffer vuoto
		return 1; // invece di fare while-wait, faccio if-return

	pthread_mutex_lock( &gPtr->mutex );
	*bPtr = gPtr->vettore[gPtr->testa];
	gPtr->testa = (gPtr->testa + 1) % DIM_BUFF;
	pthread_mutex_unlock( &gPtr->mutex );

	return 0;
}

void* produttore(void *arg){
	GestioneIO *gPtr = (GestioneIO*)arg;

	int indirizzo = -1, dato = -1;
	for(int i = 0; i < NUM_PROD; ++i){
		if(indirizzo < 0 && dato < 0){
			indirizzo = rand() % 11;
			dato = rand() % 11;
		} else {
			indirizzo += 1;
			dato += 1;
		}

		Buffer b;
		b.indirizzo = indirizzo;
		b.dato = dato;

		produci(gPtr, &b);

		printf("PRODUTTORE: Ho prodotto <indirizzo: %d, dato: %d>\n", b.indirizzo, b.dato);

		pthread_cond_signal( &gPtr->NOT_EMPTY ); // inutile dato che consuma() non è bloccante su NOT_EMPTY
		sleep(1);
	}

	pthread_exit(NULL);
}

void* consumatore(void *arg){
	GestioneIO *gPtr = (GestioneIO*)arg;

	for(int i = 0; i < NUM_CONS; ++i){
		Buffer b;
		b.indirizzo = -1;
		b.dato = -1;

		int empty = 0;
		while( !empty ){ // se ho consumato, consuma() ritorna 0
			empty = consuma(gPtr, &b);

			if( !empty )
				printf("CONSUMATORE: Ho consumato <indirizzo: %d, dato: %d>\n", b.indirizzo, b.dato);
		}

		pthread_cond_signal( &gPtr->NOT_FULL ); // sblocca i produttori

		sleep(3);
	}

	pthread_exit(NULL);
}
