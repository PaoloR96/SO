#include "header.h"
#include <pthread.h>
#include <stdio.h> // printf
#include <stdlib.h> // NULL

void inizializza(Vettore *vPtr, unsigned int new_size){
	vPtr->ptr = malloc(sizeof(int) * new_size);
	vPtr->size = new_size;

	pthread_mutex_init( &vPtr->mutex, NULL );
	pthread_cond_init( &vPtr->not_full, NULL );
	pthread_cond_init( &vPtr->not_empty, NULL );
}

void distruggi(Vettore *vPtr){
	pthread_cond_destroy( &vPtr->not_empty );
	pthread_cond_destroy( &vPtr->not_full );
	pthread_mutex_destroy( &vPtr->mutex );

	free(vPtr->ptr);
}

void inserisci_elemento(Vettore *vPtr, int elem){
	pthread_mutex_lock( &vPtr->mutex );

	while( vPtr->index == vPtr->size ) // vettore pieno
		pthread_cond_wait( &vPtr->not_full, &vPtr->mutex );

	vPtr->ptr[vPtr->index] = elem;
	vPtr->index += 1;

	printf("[MANAGER] Ho inserito %d in coda\n", elem);

	pthread_cond_signal( &vPtr->not_empty );
	pthread_mutex_unlock( &vPtr->mutex );
}

int preleva_elemento(Vettore *vPtr){
	pthread_mutex_lock( &vPtr->mutex );

	while( vPtr->index == 0 ) // vettore vuoto
		pthread_cond_wait( &vPtr->not_empty, &vPtr->mutex );

	vPtr->index -= 1;
	int temp = vPtr->ptr[vPtr->index];

	printf("[WORKER] Ho prelevato %d dalla coda\n", temp);

	pthread_cond_signal( &vPtr->not_full );
	pthread_mutex_unlock( &vPtr->mutex );

	return temp;
}
