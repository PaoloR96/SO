#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "header.h"

void* doit(void *arg){
	Monitor *m = (Monitor*)arg;

	int cp = affitta(m, 1);
	sleep(1);
	restituisci(m, 1, cp);
	
	cp = affitta(m, 2);
	sleep(1);
	restituisci(m, 2, cp);
	
	cp = affitta(m, 3);
	sleep(1);
	restituisci(m, 3, cp);
	
	pthread_exit(NULL);
}

void* print(void *arg){
	Monitor *m = (Monitor*)arg;
	
	for(int i = 0; i < 20; ++i){
		sleep(1);
		stampa(m);
		printf("\n");
	}
	
	pthread_exit(NULL);
}

int main(){
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	Monitor *mPtr = (Monitor*)malloc( sizeof(Monitor) );

	inizializza(mPtr);

	pthread_t threads[ 4 + 1 ];
	
	for(int i = 0; i < 4; ++i){
		pthread_create( &threads[i], &attr, doit, (void*)mPtr );
	}
	
	pthread_create( &threads[4], &attr, print, (void*)mPtr );
	
	for(int i = 0; i < 4 + 1; ++i)
		pthread_join( threads[i], NULL );
	
	pthread_attr_destroy( &attr );
	free(mPtr);
	pthread_exit(NULL);
}
