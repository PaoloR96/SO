#ifndef HEADER_H
#define HEADER

/*
 Applicazione multiprocesso.
 20 processi (10 fornitori, 10 clienti)
*/

// quanti figli istanziare nel main
#define NUM_FORNITORI 10
#define NUM_CLIENTI 10
#define NUM_PROC NUM_FORNITORI+NUM_CLIENTI

// grandezza del vettore condiviso
#define SIZE_MAGAZZINO 100

// stati degli scaffali
#define STATO_LIBERO 0
#define STATO_OCCUPATO 1
#define STATO_INUSO 2

// variabili condition del monitor
#define OK_FORNITORE 0
#define OK_CLIENTE 1

typedef struct {
	unsigned int id_fornitore;	// pid dell'ultimo fornitore che ha usato quello scaffale
	unsigned int stato;		// libero, occupato, in_uso
} Scaffale;

typedef struct {
	Scaffale s[SIZE_MAGAZZINO];	// array di 100 scaffali
	unsigned int livello_scorte;	// valore max = SIZE_MAGAZZINO
} Magazzino;

#include "monitor.h"

void fornitore(Magazzino*, Monitor*);
void cliente(Magazzino*, Monitor*);

void init_magazzino(Magazzino*);

#endif
