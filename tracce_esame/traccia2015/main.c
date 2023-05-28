#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>

#include "header.h"

int main(){
	pthread_attr_t attr;
	pthread_t pthreads[NUM_THREAD_PROD + NUM_THREAD_CONS];

	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

	GestioneIO *gestoreIO = malloc(sizeof(GestioneIO));
	inizializza(gestoreIO);

	srand(time(NULL));
	for(int i = 0; i < NUM_THREAD_PROD; ++i)
		pthread_create( &pthreads[i], &attr, produttore, (void*)gestoreIO );

	for(int i = NUM_THREAD_PROD; i < NUM_THREAD_PROD + NUM_THREAD_CONS; ++i)
		pthread_create( &pthreads[i], &attr, consumatore, (void*)gestoreIO );

	pthread_attr_destroy( &attr );

	for(int i = 0; i < NUM_THREAD_PROD + NUM_THREAD_CONS; ++i)
		pthread_join( pthreads[i], NULL );

	rimuovi(gestoreIO);
	free(gestoreIO);

	pthread_exit( NULL );
}
