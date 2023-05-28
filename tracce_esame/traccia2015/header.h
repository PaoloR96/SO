#ifndef HEADER_H_
#define HEADER_H_

/* Applicazione Multithread e con l'uso di Monitor */
/* Gestione di un vettore di buffer I/O disciplinati dal produttore/consumatore con vettore circolare */

#define NUM_THREAD_PROD 4
#define NUM_THREAD_CONS 2
#define NUM_PROD 3
#define NUM_CONS 4

#define DIM_BUFF 10

typedef struct {
	int indirizzo;
	int dato;
} Buffer;

#include <pthread.h>
#include <sys/types.h>
typedef struct {
	Buffer vettore[DIM_BUFF];
	/* variabili per la sincronizzazione */
	int testa;
	int coda;
	pthread_mutex_t mutex;
	pthread_cond_t NOT_FULL;
	pthread_cond_t NOT_EMPTY;
} GestioneIO;

/* imposta il valore iniziale delle variabili interne al monitor */
void inizializza(GestioneIO*);
/* rimuove le variabili del monitor */
void rimuovi(GestioneIO*);
/* Buffer* parametro di ingresso
	Deve sospendere il thread chiamante se non c'è spazio */
void produci(GestioneIO*, Buffer*);
/* Buffer* parametro di uscita
	NON deve sospendere il thread chiamante se non ci sono elementi
	Ritorna 1 se non ci sono elementi, 0 altrimenti (va a buon fine il consumo) */
int consuma(GestioneIO*, Buffer*);

/* Produce un intero fra 0 e 10 random sia per indirizzo che per dato, poi questo valore viene incrementato di 1 nelle iterazioni successive
	Fa NUM_PROD (3) iterazioni -> sleep(1) */
void* produttore(void*);
/* Consuma l'intero buffer memorizzato in ogni iterazione, stampando a video i valori ottenuti
	Fa NUM_CONS (4) iterazioni -> sleep(3) */
void* consumatore(void*);

#endif
