#ifndef _lib_
#define _lib_

//dimendioni dela matrice
#define DIM_X 8
#define DIM_Y 6
//definisco il numero di threads da creare
#define NUM_THREADS DIM_Y

//mutex
pthread_mutex_t MUTEX;
pthread_cond_t FINE;

//valore da assegnare alle cella dell'arrey di stato nel Buffer
enum{LIBERO, OCCUPATO};

//creo un tipo Matrice
typedef int Matrice[DIM_X][DIM_Y];

typedef struct{
	Matrice matrice;
	int array_somma[DIM_Y];
	int array_stato[DIM_Y];
	int dimensione;
	int risultato;
}Buffer;

void *ThreadMaster(void*);
void *Threads(void*);

int ricerca(Buffer*);

#endif