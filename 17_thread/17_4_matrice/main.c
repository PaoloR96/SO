/*
 *			ESERCITAZIONE 
 *	Scrivere un’applicazione concorrente per il calcolo della
 *	somma degli elementi contenuti in una matrice.
 *
 *	I Threads T_i accedono al buffer vettoreSomma per la somma
 *	della colonna i-sima in mutua esclusione senza alcun
 *	vincolo sulla posizione
 *	(e.s. Il thread P2 termina per primo e scrive
 *	nella casella vettoreSomma[0]; il thread P1 finisce per
 *	secondo e scrive nella casella vettoreSomma[1], ecc.)
 *
 *
 *	Il thread T (per la somma totale) resta in attesa
 *	della terminazione di tutti i Thread T_i.
 *	Il Thread padre (diverso da T) attende la
 *	terminazione di tutti i processi e restituisce a
 *	video il totale calcolato da T.
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "thread.h"

int main(){

	//variabili
	pthread_attr_t attr;
	pthread_t threads[NUM_THREADS];
	Buffer *buffer;
	int i;
	int x;
	int y;
	
	printf("\n\n _INIZIO_ \n\n");
	
	//alloco memoria e inizializzo
	buffer = (Buffer*)malloc(sizeof(Buffer));
	buffer->dimensione = 0;
	buffer->risultato = 0;
	
	for(i=0; i<DIM_Y; i++){
		buffer->array_somma[i] = 0;
		buffer->array_stato[i] = LIBERO;
	}
	
	srand(time(NULL));
	for(x=0; x<DIM_X; x++){
		for(y=0; y<DIM_Y; y++){
			buffer->matrice[x][y] = rand()%100;
			printf("%d\t", buffer->matrice[x][y]);
		}
			printf("\n");
	}
	printf("\n");
	
	//inizializo monitor thread
	pthread_mutex_init(&MUTEX, NULL);
	pthread_cond_init(&FINE, NULL);
	
	//inizializzo attributi thread joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//creazione dei thread
	for(i=0; i<NUM_THREADS+1; i++){
		if( i== 0){
				//Sono il ThreadMaster
				printf("Sono il Thread Master id : <%d>\n",i);
				pthread_create(&threads[i], &attr, ThreadMaster, (void*)buffer);
		}else{
				//Threads
				printf("Sono il Thread id : <%d>\n",i);
				pthread_create(&threads[i], &attr, Threads, (void*)buffer);
		}
	
	}
	
	for(i=0; i<NUM_THREADS+1; i++){
		pthread_join(threads[i],NULL);
		printf("Thread TERMINATO\n");
	}
	
	//stampo l'arrey di somma
	printf("\narray di somma : \n  ");
	for(i=0; i<DIM_Y; i++){
		printf("%d\t", buffer->array_somma[i]);
	}
	printf("\n");
	
	//stampo il risultato finale
	printf("\nla somma totale è : %d", buffer->risultato);
	
	printf("\n\n _FINE_ \n\n");
	
	//libero memoria occupata 
	free(buffer);
	pthread_mutex_destroy(&MUTEX);
	pthread_cond_destroy(&FINE);
	
	pthread_exit(0);
}
