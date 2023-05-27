#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "thread.h"

//procedure gestione produttore e consumatore
void inizio_Produzione(Buffer *buffer){
	pthread_mutex_lock(&MUTEX);	
	if(buffer->stato == PIENO){
		pthread_cond_wait(&OK_PRODUCI, &MUTEX);
	}	
	pthread_mutex_unlock(&MUTEX);
}

void fine_Produzione(Buffer *buffer){
	pthread_mutex_lock(&MUTEX);
	buffer->stato = PIENO;
	pthread_cond_signal(&OK_CONSUMA);
	pthread_mutex_unlock(&MUTEX);
	
}

void inizio_Consumazione(Buffer *buffer){
	pthread_mutex_lock(&MUTEX);
	if(buffer->stato == VUOTO){
		pthread_cond_wait(&OK_CONSUMA, &MUTEX);
	}
	pthread_mutex_unlock(&MUTEX);
}

void fine_Consumazione(Buffer *buffer){
	pthread_mutex_lock(&MUTEX);
	buffer->stato = VUOTO;
	pthread_cond_signal(&OK_PRODUCI);
	pthread_mutex_unlock(&MUTEX);
}

//procedure che invocheranno i thread
void *Consumatore(void *b){
	Buffer*buffer = (Buffer*)b;
	inizio_Consumazione(buffer);

	printf(">Consumo Dato <%d>\n",buffer->dato);
	buffer->dato = 0;

	fine_Consumazione(buffer);
}

void *Produttore(void *b){
	Buffer*buffer = (Buffer*)b;
	inizio_Produzione(buffer);
	
	buffer->dato = rand()%100;
	printf(">Produco Dato <%d>\n",buffer->dato);
	
	fine_Produzione(buffer);
}

