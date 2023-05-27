#ifndef _LIB_
#define _LIB_

//semafori
#define MUTEX_CONS 0
#define MSG_DISP 1
#define SPAZIO_DISP 2

//dimensione della coda
#define DIM 5

//numero di produzioni e di consumazioni
#define NUM_PROD 6 
#define NUM_CONS 2

//numero di processi
#define NUM_PROCESSI 4

typedef struct{
	int vettore[DIM];
	int testa;
	int coda;
}Buffer;

//gestione dei semafori
void wait(int,int);
void signal(int,int);

//gestione produttori consumatori
void produttore(int,Buffer*);
void consumatore(int,Buffer*);

#endif /* _LIB_ */
