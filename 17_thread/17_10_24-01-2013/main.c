#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "header.h"
/*
Il programma deve istanziare 4 thread, ciascuno dei quali: richiede una copia del film “1”, attende un secondo, e la restituisce; richiede una copia del film “2”, attende un secondo, e la restituisce; richiede una copia del film “3”, attende un secondo, e la restituisce.
*/
void *Cliente(void *args){
	Monitor *m = (Monitor*) args;

	int i, id_copia;
	for(i = 0; i < 3; i++){
		id_copia = affitta(m, i+1);
		sleep(1);
		restituisci(m, i+1, id_copia);
	}

	pthread_exit(NULL);
}

/*
Il programma deve inoltre istanziare un quinto thread che ogni secondo richiama il metodo “stampa” che mostra lo stato dei DVD del negozio.
*/
void *Stato(void *args){
	Monitor *m = (Monitor*) args;

	int i = 0;
	while(i < 6){
		stampa(m);
		sleep(1);
		i++;
	}

	pthread_exit(NULL);
}

int main(){
	int i, rc;
	pthread_t threads[5];
	pthread_attr_t attr;

	Monitor *ME;

	ME = (Monitor*)malloc(sizeof(Monitor));
	
	int k;

	printf("DVD: ");
	for(i = 0; i < 6; i++){
		ME->dvd[i].stato = DISPONIBILE;
		ME->dvd[i].identificativo_film = 1 + i%3;
		ME->dvd[i].identificativo_copia = 1 + i%2;
		printf("%d ", 1 + i%3);
	}
	printf("\n");
	
	pthread_mutex_init(&ME->MUTEX, NULL);
	pthread_cond_init(&ME->FILM1, NULL);
	pthread_cond_init(&ME->FILM2, NULL);
	pthread_cond_init(&ME->FILM3, NULL);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	
	for(i = 0; i < 5; i++){
		if(i == 0){
			printf("Creazione del thread stato...\n");
			rc = pthread_create(&threads[i], &attr, Stato, (void*) ME);
			if(rc){
				perror("Impossibile creare il thread.\n");
				exit(-1);
			}
		} else {
			printf("Creazione del thread cliente numero %d...\n", i - 1);
			rc = pthread_create(&threads[i], &attr, Cliente, (void*) ME);
			if(rc){
				perror("Impossibile creare il thread.\n");
				exit(-1);
			}
		}
	}
	
	pthread_attr_destroy(&attr);

	for(i = 0; i < 5; i++){
		pthread_join(threads[i], NULL);
		printf("Il thread numero %d è terminato.\n", i);
	}

	pthread_mutex_destroy(&(ME->MUTEX));
	pthread_cond_destroy(&(ME->FILM1));
	pthread_cond_destroy(&(ME->FILM2));
	pthread_cond_destroy(&(ME->FILM3));

	free(ME);

	pthread_exit(NULL);

	return 0;	
}
