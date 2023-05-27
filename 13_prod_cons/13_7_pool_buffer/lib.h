#ifndef __MIALIB__
#define __MIALIB__

//semafori
#define SPAZIO_DISP 0
#define MSG_DISP 1
#define MUTEX_PROD 2
#define MUTEX_CONS 3

//dimensione array
#define DIM 5

//numero di processi, consumazioni e produzioni
#define NUM_PROC 4
#define NUM_CONS 1
#define NUM_PROD 2


//gestione array si stato
#define VUOTO 0
#define PIENO 1
#define IN_USO 2

typedef struct {
	int vettore[DIM];
}Buffer;

typedef struct {
	int vettoreStato[DIM];
}BufferStato;	

//gestione semafori
void signal(int,int);
void wait(int,int);

//gestione produttori e consumatori
void produttore(int,Buffer*,BufferStato*);
void consumatore(int,Buffer*,BufferStato*);

#endif /* __MIALiB__ */
