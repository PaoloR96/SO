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

#include <pthread.h>	//per i tipi da inserire nella struct

#define NUM_CAPOTRENI 4
#define NUM_TRENI 4
#define NUM_VIAGGIATORI 10
#define NUM_STAZIONI 10

//Poi, come da traccia:

struct monitor_treno {
	int stazione;	//riporta la stazione in cui si trova il treno (tra 0 e 10)
	//VARIABILI PER LA SINCRONIZZAZIONE
	//Cosa serve per la sincronizzazione: un mutex per entrare nel monitor e poi c'è mutua esclusione tra i capitreno, quindi serve un mutex
	//per gestirla, che istanziamo come variabile condition (?)
	pthread_mutex_t MUTEX;
	pthread_cond_t CV_LETTORI;
	pthread_cond_t CV_SCRITTORI;
	int num_lett;
	int num_scritt;
	int stazione_occupata;	//Variabile sulla quale fanno il controllo i capitreno
	int id_treno;
};

void inizializza(struct monitor_treno*);
void rimuovi(struct monitor_treno*);
int leggi_stazione(struct monitor_treno*);	//restituisce la posizione attuale del treno, e PIU' VIAGGIATORI POSSONO LEGGERE LA POSIZIONE 
						//IN CONTEMPORANEA
void scrivi_stazione(struct monitor_treno*, int);	//permette ai CAPITRENO di aggiornare la posizione del treno, GESTENDO LA
								//MUTUA ESCLUSIONE TRA THREAD

//Funzioni dei thread

void *Capotreno(void*);
void *Viaggiatore(void*);
