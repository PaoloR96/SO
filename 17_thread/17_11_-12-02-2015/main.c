#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include "header.h"

int main(){
	int i, rc;
	pthread_attr_t attr;	
	//int status;	per lo stato dei thread quando li joini

	MonitorElaborazioni *ME;
	ME = (MonitorElaborazioni*) malloc(sizeof(MonitorElaborazioni)); //RIP, fucking dies - cit. Gennarot
	//malloc
	ME->cellepiene = 0;
	ME->cellevuote = 5;
	
	int k;
	for(k = 0; k < 5; k++)
		ME->VettStato[k] = VUOTO;

	pthread_t threads[NUM_THREADS];

	pthread_mutex_init(&(ME->MUTEX), NULL);
	pthread_cond_init(&(ME->ok_produzione), NULL);
	pthread_cond_init(&(ME->ok_consumo), NULL);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(i = 0; i < NUM_THREADS; i++){
		if(i < 4){
			printf("Creazione del thread produttore numero %d...\n", i + 1);
			rc = pthread_create(&threads[i], &attr, Produttore, (void*) ME);
			if(rc){
				perror("Impossibile creare il thread.\n");
				exit(-1);
			}
		} else {
			printf("Creazione del thread consumatore numero %d...\n", i - 3);
			rc = pthread_create(&threads[i], &attr, Consumatore, (void*) ME);
			if(rc){
				perror("Impossibile creare il thread.\n");
				exit(-1);
			}
		}
	}
	
	pthread_attr_destroy(&attr);

	for(i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
		printf("Il thread numero %d è terminato.\n", i);
	}

	pthread_mutex_destroy(&(ME->MUTEX));
	pthread_cond_destroy(&(ME->ok_produzione));
	pthread_cond_destroy(&(ME->ok_consumo));

	free(ME);

	pthread_exit(NULL);

	return 0;	
}
