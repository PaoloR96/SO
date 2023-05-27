#ifndef __LIB__
#define __LIB__

#define MUTEX_CONS 0
#define MUTEX_PROD 1
#define SPAZIO_DISP 2
#define MSG_DISP 3

#define DIMENSIONE 5
#define NUM_PROCESSI 4
#define NUM_PRODUZIONI 2
#define NUM_CONSUMAZIONI 1

typedef struct {
	int vettore[DIMENSIONE];
	int testa;
	int coda;
}Buffer;

void WaitSem(int,int);
void SignalSem(int,int);

void Produttore(int,Buffer*);
void Consumatore(int,Buffer*);

#endif
