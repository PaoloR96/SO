#ifndef _tlib_
#define _tlib_

#define DIM 5
#define NUM_THREADS 10

pthread_mutex_t MUTEX;
pthread_cond_t MSG_DISP;
pthread_cond_t SPAZIO_DISP;

enum{VUOTO, PIENO, IN_USO};

typedef struct{
	int array[DIM];
	int array_stato[DIM];
	int num_produttori;
	int num_consumatori;
	int dimensione;
}Buffer;

typedef struct{
	Buffer *buffer;
	int num_thread;
}Dato;

int ricerca_indice_vuoto(Buffer*);
int ricerca_indice_pieno(Buffer*);

void *Produttore(void *);
void *Consumatore(void *);

#endif
