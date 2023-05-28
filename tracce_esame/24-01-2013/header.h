#ifndef HEADER
#define HEADER

#define DISPONIBILE 0
#define AFFITTATO 1

#define NUM_DVD 6

#include <pthread.h>
#include <sys/types.h>

typedef struct {
	int id_film; // intero fra 1 e 3
	int id_copia; // intero fra 1 e 2
	int stato; // DISPONIBILE (0) o AFFITTATO (1)
} DVD;

typedef struct {
	DVD dvd[NUM_DVD]; // array di 6 DVD
	
	pthread_mutex_t mutex; // accesso ai dvd in mutua esclusione
	
	pthread_cond_t disponibile; // variabile condition
} Monitor;

int affitta(Monitor *mPtr, int id_film);
void restituisci(Monitor *mPtr, int id_film, int id_copia);
void stampa(Monitor *mPtr);

void inizializza(Monitor *mPtr);

#endif
