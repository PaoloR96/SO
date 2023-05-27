/*
Si realizzi in linguaggio C o C++ un programma multi-thread (con la libreria PThreads) per la estione di un servizio di noleggio film in DVD. Il programma dovrà essere basato sul costrutto Monitor, usando la seguente interfaccia:

Nel negozio sono disponibili 6 DVD, rappresentati da un vettore di strutture “DVD”. Ad ogni DVD del negozio è assegnato un intero “identificativo_film” che rappresenta il film contenuto nel DVD (si supponga che ci siano 3 film disponibili, identificati da un numero tra 1 e 3). Inoltre, per uno stesso film possono essere disponibili più copie, ed ogni copia ha un intero “identificativo_copia” che la distingue (si supponga che per ogni film di siano 2 copie, identificate da un numero pari a 1 o 2). Ad esempio, le 2 copie del film “1” avranno un “identificativo_copia” rispettivamente uguale a “1” e “2”; le 2 copie del film “2” avranno “identificativo_copia” uguale a “1” e “2”; etc.

Il metodo “affitta” accetta in ingresso l’identificativo del film da affittare. Esso cerca una copia del film disponibile, e se la trova la pone nello stato AFFITTATO. Se non la trova, il thread richiedente è messo in attesa finché non si rende disponibile una copia di quello specifico film. Il metodo deve ritornare lo identificativo_copia del DVD selezionato. Il metodo “restituisci” accetta in ingresso
l’identificativo del film e della copia presa in affitto, la pone nello stato DISPONIBILE, e rimuove dalla attesa un eventuale thread in attesa di quel film, permettendogli di affittarlo.

Il programma deve istanziare 4 thread, ciascuno dei quali: richiede una copia del film “1”, attende un secondo, e la restituisce; richiede una copia del film “2”, attende un secondo, e la restituisce; richiede una copia del film “3”, attende un secondo, e la restituisce. Il programma deve inoltre istanziare un quinto thread che ogni secondo richiama il metodo “stampa” che mostra lo stato dei DVD del negozio.
*/

#include <pthread.h>

#define DISPONIBILE 0
#define AFFITTATO 1

typedef struct {
	int identificativo_film;
	// un intero tra 1 e 3
	int identificativo_copia;
	// un intero tra 1 e 2
	int stato;
	// DISPONIBILE=0, oppure AFFITTATO=1
} DVD;

typedef struct {
	// vettore di DVD da gestire
	DVD dvd[6];

	//mutex per il monitor	
	pthread_mutex_t MUTEX;

	//una per film
	pthread_cond_t FILM1;
	pthread_cond_t FILM2;
	pthread_cond_t FILM3;

	//tre contatori che partono da 2
	/*int num_film1;
	int num_film2;
	int num_film3;*/
} Monitor;

void *Cliente(void*);
void *Stato(void*);

int affitta(Monitor *, int);
void restituisci(Monitor *, int, int);
void stampa(Monitor *);
