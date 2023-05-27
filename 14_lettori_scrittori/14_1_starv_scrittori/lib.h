#ifndef __LIBRERY__
#define __LIBRERY__

//semafori 
#define SYNCH 0
#define MUTEX_LETTORI 1


//numero procesi numero produzioni e consumazioni dimensione buffer
#define DIM 5
#define NUM_PROC 6

typedef struct{
	int dato;
	int num_lettori;
	int cont_lettura;
}Buffer;

//gestione dei semafori
void signal(int,int);
void wait(int,int);

//funzioni richiamate dalle funzioni di lettura e scrittura
void inizioLettura(int,Buffer*);
void fineLettura(int,Buffer*);

//quello che faranno i processi lettore e scrittore
void scrittore(int,Buffer*);
void lettore(int,Buffer*);

#endif /* __LIBRERY__ */
