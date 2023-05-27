/*
 * mialib.h
 *
 *  Created on: 01 nov 2016
 *      Author: dome
 */

#ifndef __HEADER__
#define __HEADER__

#include "elemento.h"

/*
 * viene inclusa per camniare il tipo di STack da usare
 * baststa cambiare il tipo a taypedef presente nella libreria elemento.h
 * per cambiare il tipo di stack da utilizzare (int,float,long int ... ),
 * e anche la dimensione dello satck.
 *
 */



typedef struct {
	elemento *vett;			//capire bene il puntatore al tipo di struttura dati in questo caso il tipo è elemento vedere il tipo in 						  elemento.h
	int dim;
	int sp;
}Stack;

void inizzializza(Stack,int);
void push(Stack,elemento);
void stampaStack(Stack);
elemento pop(Stack);
int dimStack(Stack);

#endif
