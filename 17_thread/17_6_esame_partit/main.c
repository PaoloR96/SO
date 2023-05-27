#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "mialib.h"

void *Scrittura_a(void *partita){
	buffer *p = (buffer*) partita;
	int i;
	for(i=0; i<6; i++){
		goal_a(p);
		sleep(2);
	}
	pthread_exit(0);
}

void *Scrittura_b(void *partita){
	buffer *p = (buffer*) partita;
	int i;
	for(i=0; i<6; i++){
		goal_b(p);
		sleep(2);
	}
	pthread_exit(0);
}

void *Lettura(void *partita){
	buffer *p = (buffer*) partita;
	int i;
	for(i=0; i<18; i++){
		utente(p);
		sleep(1);
	}
	pthread_exit(0);
}

int main(){

	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	buffer *p;
	int i;
	
	printf("\n\n _INIZIO_ \n\n");
	
	p = (buffer*)malloc(sizeof(buffer));
	p->goal_a = 0;
	p->goal_b = 0;
	p->num_lettori = 0;
	p->somma_a = 0;
	p->somma_b = 0;
	p->stato = LIBERO;
	
	pthread_mutex_init(&(p->mutex),NULL);
	pthread_cond_init(&(p->leggi),NULL);
	pthread_cond_init(&(p->scrivi),NULL);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(i=0; i<NUM_THREADS; i++){
		srand(i+time(NULL));
		if(i==0){
			//thread_a
			pthread_create(&threads[i], &attr, Scrittura_a, (void*)p);
		}
		if(i == 1){
			//thread_b
			pthread_create(&threads[i], &attr, Scrittura_b, (void*)p);
		}
		
	if( i >=2 && i<=NUM_THREADS){
			//thread_utenti
			pthread_create(&threads[i], &attr, Lettura, (void*)p);
		}
	}
	
	for(i=0; i<NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	
	if(p->somma_a > p->somma_b){
		printf("vince A  !! \n");
	}
	if(p->somma_a < p->somma_b){
		printf("vince B  !! \n");
	}
	if(p->somma_a == p->somma_b){
		printf("pareggio  !! \n");
	}
	
	
	pthread_cond_destroy(&(p->leggi));
	pthread_cond_destroy(&(p->scrivi));
	pthread_mutex_destroy(&(p->mutex));
	pthread_attr_destroy(&attr);
	free(p);
	printf("\n\n _FINE_ \n\n");
	
	return 0;
}
