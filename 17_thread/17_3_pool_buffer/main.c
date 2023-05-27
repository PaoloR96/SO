/*	Produttore Consumatore 
 *	Abbiamo 1 array da riempire che puo essere riempito simultamentamente
 *	su piu celle, e i consumatori lo svuotano, anche loro possono
 *	accedere contemporaneamente alla risorsa 
 */

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread.h"

int main(){

	//dichiarazione delle varaibili
	pthread_attr_t attr;
	pthread_t threads[NUM_THREADS];
	Buffer *buffer;
	Dato *dato;
	int i,k;

	printf("\n\n _INIZIO_ \n\n");

	//creo buffer nell'area hype che useranno i thread e la inizializzo
	buffer = (Buffer*)malloc(sizeof(Buffer));
	buffer->num_consumatori = 0;
	buffer->num_produttori = 0;
	buffer->dimensione = DIM;
	for(i=0; i<DIM; i++){
		buffer->array[i] = 0;
		buffer->array_stato[i] = VUOTO;
	}

	dato = (Dato*)malloc(sizeof(Dato));
	dato->buffer = buffer;
	dato->num_thread = 0;

	//inizializzo i thread
	pthread_mutex_init(&MUTEX,NULL);
	pthread_cond_init(&SPAZIO_DISP,NULL);
	pthread_cond_init(&MSG_DISP,NULL);

	//imposto i threads joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//creazione dei thread
	for(i=0; i<NUM_THREADS; i++){
		if( i%2 == 0){
			//Produttori
			printf("Sono Produttore  n: <%d> \n",i);
			dato->num_thread = i;
			pthread_create(&threads[i], &attr, Produttore, (void*)dato);
		
		}else{
			//Consumatori
			printf("Sono Consumatore n: <%d> \n",i);
			dato->num_thread = i;
			pthread_create(&threads[i], &attr, Consumatore, (void*)dato);
		}
	}

	//join dei threads
	for(i=0; i<NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
		printf("Threads Terminato\n");
	}	

	//rimozione memoria e thread
	free(buffer);
	pthread_mutex_destroy(&MUTEX);
	
	printf("\n\n _FINE_ \n\n");

	return 0;
}
