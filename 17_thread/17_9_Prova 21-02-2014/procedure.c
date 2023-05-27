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
#include <sys/types.h>
#include <sys/ipc.h>

#include "header.h"

void inizializza(struct monitor_treno *m){

	pthread_mutex_init(&(m->MUTEX), NULL);
	pthread_cond_init(&(m->CV_LETTORI), NULL);
	pthread_cond_init(&(m->CV_SCRITTORI), NULL);

	m->stazione = 0;
	m->stazione_occupata = 0;
	m->num_lett = 0;
	m->num_scritt = 0;
}

void rimuovi(struct monitor_treno *m){
	pthread_mutex_destroy(&(m->MUTEX));
	pthread_cond_destroy(&(m->CV_LETTORI));
	pthread_cond_destroy(&(m->CV_SCRITTORI));

	free(m);
}

int leggi_stazione(struct monitor_treno *m){

	pthread_mutex_lock(&(m->MUTEX));	//ci vuole il mutex?

	m->num_lett++;

	while(m->stazione_occupata == 1)
		pthread_cond_wait(&(m->CV_LETTORI), &(m->MUTEX));

	pthread_cond_signal(&(m->CV_LETTORI));
	pthread_mutex_unlock(&(m->MUTEX));

	int val;
	val = m->stazione;
	
	pthread_mutex_lock(&(m->MUTEX));
	
	m->num_lett--;

	if(m->num_lett == 0)
		pthread_cond_signal(&(m->CV_SCRITTORI));

	pthread_mutex_unlock(&(m->MUTEX));
	
	return val;
}

void scrivi_stazione(struct monitor_treno *m, int stazione){
	srand(time(0));

	pthread_mutex_lock(&(m->MUTEX));

	m->num_scritt++;

	while(m->stazione_occupata == 1 || m->num_lett > 0)
		pthread_cond_wait(&(m->CV_SCRITTORI), &(m->MUTEX));
	
	m->stazione_occupata = 1;

	pthread_mutex_unlock(&(m->MUTEX));

	m->stazione = stazione;
	printf("[CAPOTRENO] - Il treno %d ha raggiunto la stazione %d.\n", m->id_treno, m->stazione);

	pthread_mutex_lock(&(m->MUTEX));

	m->stazione_occupata = 0;
	m->num_scritt--;
	
	if(m->num_scritt > 0)
		pthread_cond_signal(&(m->CV_SCRITTORI));
	else
		pthread_cond_signal(&(m->CV_LETTORI));
	
	pthread_mutex_unlock(&(m->MUTEX));
}

//Se ogni capotreno istanza un monitor_treno diverso e lavora su quello, come facciamo a far scegliere un monitor_treno ai viaggiatori?
//Se dichiaro un vettore di monitor_treno e passo un indice al capotreno per istanziare QUEL monitor_treno in QUELLA POSIZIONE?

//for(creazionethreads){
//	if(i < 4){
//		monitor_treno *monitr0 = vett_monitor_treni[i];
//		passo questo monitr0 alla pthread_create }

void *Capotreno(void* mt){
	struct monitor_treno *monitr = (struct monitor_treno*) mt;

	int stazione;

	for(stazione = 1; stazione <= NUM_STAZIONI; stazione++){
		scrivi_stazione(monitr, stazione);
		sleep(3);
	}

	pthread_exit(NULL);
}

//La scelta del monitor casuale la facciamo nel main
//	else {
//		srand(time(0));
//		val = rand()%4;
//		if(val == 0)
//			pthread_create con monitor0;
//		else if(val == 1)
//			... ci siamo capiti

void *Viaggiatore(void* mt){
	struct monitor_treno *monitr = (struct monitor_treno*) mt;
	int k;
	int val;
	//int numero_treno = (int) arg;
	int tot = 3;
	srand(time(0));

	for(k = 0; k < tot; k++){
		sleep(1 + rand()%6);
		val = leggi_stazione(monitr);
		printf("[VIAGGIATORE] - Il treno %d si trova nella stazione %d.\n", monitr->id_treno, val);
	}

	pthread_exit(NULL);
}
