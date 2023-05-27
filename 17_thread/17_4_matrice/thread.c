#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "thread.h"

void *ThreadMaster(void *g){
	
	Buffer *buffer = (Buffer*)g;
	int i;
	
	//blocco il monitor 
	pthread_mutex_lock(&MUTEX);
	
	//attendo una signal su FINE
	pthread_cond_wait(&FINE, &MUTEX);
	
	printf("Sono Thread Master faccio la somma totale\n");
	for(i=0; i<buffer->dimensione; i++){
		//faccio la somma dell'array e la salvo in una variabile del Buffer
		buffer->risultato += buffer->array_somma[i];
	}
	
	//sblocco ed esco
	pthread_mutex_unlock(&MUTEX);
	
	//amazzo il thread
	pthread_exit(0);
}

void *Threads(void *g){
	
	Buffer *buffer = (Buffer*)g;
	int indice_colonna;
	int i;
	int somma = 0;
	
	//faccio entrare thread nel monitor per rimanere in mutua esclusione
	pthread_mutex_lock(&MUTEX);
	
	//mi ricavo la prima colonna che nessun thread stia usando 
	indice_colonna = ricerca(buffer);
	buffer->array_stato[indice_colonna] = OCCUPATO;
	
	//esco dal monitor per far entrare alti thread
	pthread_mutex_unlock(&MUTEX);
	
	//sono lunico thread a lavorare su questo indice (indice_colonna)
	for(i=0; i<DIM_Y; i++){
		//faccio la somma della colonnna e la inserisco in una variabile
		somma += buffer->matrice[i][indice_colonna];
	}
	printf("Sono Thread somma <%d> colonna [%d] \n", somma, indice_colonna);
	
	//rientro in mutua esclusione
	pthread_mutex_lock(&MUTEX);
	
	//inserisco la somma fatta nella locazione dimensione dell'array somma e incremento dimensione
	buffer->array_somma[buffer->dimensione] = somma;
	buffer->dimensione++;
	
	//se dimensione è uguale al numero di colonne ho finito e avvert o al Master che deve fare la somma 
	if(buffer->dimensione == DIM_Y){
		pthread_cond_signal(&FINE);
	}
	
	//esco dalla mutua esclusione
	pthread_mutex_unlock(&MUTEX);
	
	//ammazzo il thread
	pthread_exit(0);
}

//fa una ricerca della prima cella libera che incontra
int ricerca(Buffer *buffer){
	int i;
	for(i=0; i<DIM_Y; i++){
		if(buffer->array_stato[i] == LIBERO){
			return i;
		}
	}
	return -1;
}
