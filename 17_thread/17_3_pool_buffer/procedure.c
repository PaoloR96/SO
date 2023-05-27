#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread.h"

int ricerca_indice_pieno(Buffer *buffer){
	int i;
	for(i=0; i<DIM; i++){
		if(buffer->array_stato[i] == PIENO){ return i; }		
	}	
	return(-1);
}

int ricerca_indice_vuoto(Buffer *buffer){
	int i;
	for(i=0; i<DIM; i++){
		if(buffer->array_stato[i] == VUOTO){ return i; } 		
	}	
	return(-1);
}

void *Consumatore(void *b){
	
	//converto il dato di input nel thread con un cast
	Dato *dato = (Dato*)b;
	int indice;
	
	//blocco il mutex
	pthread_mutex_lock(&MUTEX);

	//rimango a verificare la condizione 
	while(dato->buffer->dimensione == DIM){
		pthread_cond_wait(&MSG_DISP,&MUTEX);
	}
	
	//mi prendo un indice dove è prelevo il dato da leggere
	indice = ricerca_indice_pieno(dato->buffer);

	//arrivati a questo punto ha ricevuto la signal dai produttori su OK_CONSUMA
	dato->buffer->array_stato[indice] = IN_USO;
	//incremento il numero di consumatori nel monitor
	dato->buffer->num_consumatori++;
	
	//rilascio il mutex
	pthread_mutex_unlock(&MUTEX);
	
	//Eseguo il consumo del dato
	printf(">CONSUMO Dato : <%d> in indice [%d]\n",dato->buffer->array[indice], indice);
	dato->buffer->array[indice] = 0;

	//blocco MUTEX e decremento il numero di cosumatori all'interno del mio monitor rilascio MUTEX
	pthread_mutex_lock(&MUTEX);
	dato->buffer->array_stato[indice] = VUOTO;
	dato->buffer->num_consumatori--;
	dato->buffer->dimensione++;
	pthread_cond_signal(&SPAZIO_DISP);
	pthread_mutex_unlock(&MUTEX);

	pthread_exit(NULL);
}


void *Produttore(void *b){

	Dato *dato = (Dato*)b;
	int indice;	

	pthread_mutex_lock(&MUTEX);

	//è un monitor di tipo signal end continue per questo va messo il whaile, perchè la 
	//condizione protrebbe essere non più valida alla ripresa del thread
	while(dato->buffer->dimensione == 0){
		pthread_cond_wait(&SPAZIO_DISP, &MUTEX);
	}

	indice = ricerca_indice_vuoto(dato->buffer);
	dato->buffer->array_stato[indice] = IN_USO;
	dato->buffer->num_produttori++;

	pthread_mutex_unlock(&MUTEX);

	sleep(1);

	dato->buffer->array[indice] = rand()%100;
	printf(">PRODUCO il dato <%d> in indice [%d]\n", dato->buffer->array[indice], indice);
	
	pthread_mutex_lock(&MUTEX);
	dato->buffer->array_stato[indice] = PIENO;
	dato->buffer->num_produttori--;
	dato->buffer->dimensione--;
	pthread_cond_signal(&MSG_DISP);
	pthread_mutex_unlock(&MUTEX);
	
	pthread_exit(NULL);
}

