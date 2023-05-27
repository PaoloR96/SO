/*
Programma MULTITHREAD che simuli il monitoraggio di traffico ferroviario, basato usl costrutto MONITOR.

Si monitorano 4 treni, la cui posizione è rappresentata con un valore compreso tra 0 e 10 -> la posizione viene aggioranta da dei THREAD CAPITRENO e consultata da dei THREAD VIAGGIATORI.

Ciascun treno viene monitorato da un'istanza di un monitor così definito:

struct monitor_treno {
	int stazione;	//riporta la stazione in cui si trova il treno (tra 0 e 10)
	//VARIABILI PER LA SINCRONIZZAZIONE
};

Funzioni presenti:

void inizializza(struct monitor_treno *m);
void rimuovi(struct monitor_treno *m);
int leggi_stazione(struct monitor_treno *m);	//restituisce la posizione attuale del treno, e PIU' VIAGGIATORI POSSONO LEGGERE LA POSIZIONE 
						//IN CONTEMPORANEA
void scrivi_stazione(struct monitor_treno *m, int stazione);	//permette ai CAPITRENO di aggiornare la posizione del treno, GESTENDO LA
								//MUTUA ESCLUSIONE TRA THREAD

Il programma principale dovrà istanziare 4 ISTANZE DEL MONITOR e 4 THREAD CAPITRENO (una istanza ed un thread per ogni treno). I capitreno dovranno invocare 10 volte il metodo scrivi_stazione(), incrementando stazione di 1 ad ogni invocazione e attendendo 3 secondi tra le invocazioni. Il valore di stazione inizialmente è 0.

Dovranno essere istanziati 10 thread viaggiatori, che dovranno scegliere un treno a caso e consulteranno la posizione del treno scelto per 3 volte, invocando il metodo leggi_stazione, dopo aver atteso per tra 1 e 6 secondi tra le invocazioni.
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "header.h"

int main(){

	int k, rc, x;
	int numthreads = NUM_CAPOTRENI + NUM_VIAGGIATORI;

	srand(time(0));	

	pthread_t threads[numthreads];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	struct monitor_treno *vett_mt[4];	//li alloco separatamente, fa tutto inizializzazione()

	for(k = 0; k < NUM_TRENI; k++){
		vett_mt[k] = (struct monitor_treno*) malloc(sizeof(struct monitor_treno));
		vett_mt[k]->id_treno = k;
	}

	for(k = 0; k < NUM_TRENI; k++)
		inizializza(vett_mt[k]);

	for(k = 0; k < numthreads; k++){
		if(k < 4){
			rc = pthread_create(&threads[k], &attr, Capotreno, (void*) vett_mt[k]);
			if(rc){
				perror("Impossibile creare il thread Capotreno. Terminazione...\n");
				exit(-1);
			}
		} else {
			x = rand()%4;
			rc = pthread_create(&threads[k], &attr, Viaggiatore, (void*) vett_mt[x]);	
			if(rc){
				perror("Impossibile creare il thread Viaggiatore. Terminazione...\n");
				exit(-1);
			}
		}		
	}

	pthread_attr_destroy(&attr);

	for(k = 0; k < numthreads; k++){
		pthread_join(threads[k], NULL);
		printf("Il thread numero %d ha terminato la sua esecuzione.\n", k); 	
	}

	for(k = 0; k < NUM_TRENI; k++)
		rimuovi(vett_mt[k]);

	pthread_exit(NULL);

	return 0;
}
