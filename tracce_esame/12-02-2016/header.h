#ifndef HEADER_H_
#define HEADER_H_

#include <pthread.h>

typedef struct {
	int *ptr;
	int size;
	int index; // punta alla prima cella non piena

	pthread_mutex_t mutex;
	pthread_cond_t not_full;
	pthread_cond_t not_empty;
} Vettore;

void inizializza(Vettore *, unsigned int);
void inserisci_elemento(Vettore *, int);
int preleva_elemento(Vettore *);
void distruggi(Vettore *);

#endif
