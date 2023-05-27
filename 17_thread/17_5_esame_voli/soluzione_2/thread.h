#ifndef _lib_
#define _lib_

#define DIM 3

//valore del vettore di stato
enum{LIBERO, OCCUPATO, IN_USO};

//numero di thread da generare
#define NUM_THREADS 5

typedef struct{
	int identificativo;
	int quota;
}Volo;

typedef struct{
	Volo vettore_voli[DIM];
	int vettore_stato[DIM];
	int dimensione;
	pthread_mutex_t MUTEX;
	pthread_cond_t SPAZIO_DISP;
}GestioneVoli;

void InserisciVolo(GestioneVoli *g, int identificativo);
void RimuoviVolo(GestioneVoli *g, int identificativo);
void AggiornaVolo(GestioneVoli *g, int identificativo, int quota);

void *Aeroporto( void *);

#endif
