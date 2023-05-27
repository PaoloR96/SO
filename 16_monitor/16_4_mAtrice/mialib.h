#ifndef _mialib_
#define _mialib_

#define DIM_X 3
#define DIM_Y 3

#define NUM_CONDITIONS 1
#define FINITO_DI_SOMMARE 0

typedef int Matrice[DIM_X][DIM_Y];

typedef struct {
	Monitor monitor;
	int vettoreSomma[DIM_X];
	int contatore_pos;
	int risultato;
}shm;

void sommaColonna(shm*,int, Matrice);
int sommaVettore(shm*);


#endif /* _mialib_ */
