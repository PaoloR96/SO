#ifndef _LIB_
#define _LIB_

//semafori
#define MUTEX 0
#define PRODUCI 1
#define CONSUMA 2

//dimensione buffer
#define DIM 5

//numero di processi 
#define NUM_PROC 2

//numero produzioni e consumazioni
#define NUM_PROD 3
#define NUM_CONS 3

//struttura del buffer
typedef struct {
	int vettore[DIM];
}Buffer;

//gestione sincronizzazione tra i due processi
void waitSinc(int, int );
void signalSinc(int , int );

//gestione della risorsa mutua esclusione
void waitMutex(int,int);
void signalMutex(int,int);

//gestione produttore consumatore
void produttore(int,Buffer*);
void consumatore(int,Buffer*);

#endif /* _LIB_ */
