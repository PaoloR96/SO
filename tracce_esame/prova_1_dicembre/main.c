#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

int main() {
	pthread_t prod_threads[NUM_PROD];
	pthread_t cons_threads[NUM_CONS];
	pthread_attr_t attr;
	
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );
	
	Stack *s = malloc( sizeof( Stack ) );
	
	stack_init( s, 20 ); // stack di 20 elementi
	
	for(int i = 0; i < NUM_PROD; ++i)
		pthread_create( &prod_threads[i], &attr, prod, (void*)s );
	
	for(int i = 0; i < NUM_CONS; ++i)
		pthread_create( &cons_threads[i], &attr, cons, (void*)s );
		
	for(int i = 0; i < NUM_PROD + NUM_CONS; ++i)
		if( i < NUM_PROD )
			pthread_join( prod_threads[i], NULL );
		else
			pthread_join( cons_threads[i], NULL );
	
	pthread_attr_destroy( &attr );
	stack_remove( s );
	free( s );
}
