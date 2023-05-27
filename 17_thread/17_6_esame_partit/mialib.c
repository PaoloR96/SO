#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "mialib.h"



void goal_a(buffer*p){
	pthread_mutex_lock(&(p->mutex));
	while(p->stato == OCCUPATO){
		pthread_cond_wait(&(p->scrivi), &(p->mutex));
	}
	p->stato = OCCUPATO;
	pthread_mutex_unlock(&(p->mutex));
	
	p->goal_a = rand()%2;
	p->somma_a += p->goal_a;
	printf("A ha segnato %d tot %d \n",p->goal_a, p->somma_a);
	
	pthread_mutex_lock(&(p->mutex));
	p->stato = LIBERO;
	pthread_cond_signal(&(p->leggi));
	pthread_mutex_unlock(&(p->mutex));
}

void goal_b(buffer*p){
	pthread_mutex_lock(&(p->mutex));
	while(p->stato == OCCUPATO){
		pthread_cond_wait(&(p->scrivi), &(p->mutex));
	}
	p->stato = OCCUPATO;
	pthread_mutex_unlock(&(p->mutex));
	
	p->goal_b = rand()%2;
	p->somma_b += p->goal_b;
	printf("B ha segnato %d tot %d \n",p->goal_b, p->somma_b);
	
	pthread_mutex_lock(&(p->mutex));
	p->stato = LIBERO;
	pthread_cond_signal(&(p->leggi));
	pthread_mutex_unlock(&(p->mutex));
}

void utente(buffer*p){
	pthread_mutex_lock(&(p->mutex));
	while(p->stato == OCCUPATO && p->num_lettori == 0){
		pthread_cond_wait(&(p->leggi), &(p->mutex));
	}
	p->num_lettori++;
	p->stato = OCCUPATO;
	pthread_cond_signal(&(p->leggi));
	pthread_mutex_unlock(&(p->mutex));
	
	printf("leggo A-B %d - %d \n", p->somma_a, p->somma_b);
	
	pthread_mutex_lock(&(p->mutex));
	p->num_lettori--;
	if(p->num_lettori == 0){
		p->stato = LIBERO;
		pthread_cond_signal(&(p->scrivi));
	}
	pthread_mutex_unlock(&(p->mutex));
}