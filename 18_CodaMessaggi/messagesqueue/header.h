/*
Header file per il programma chiamante e i programmi di supporto.
Si vuole realizzare:
	- un programma generatore che manda messaggi in una coda
	- un programma che riceve i messaggi, ne fa la media e stampa tutto a video
	- un programma principale che coordina questi programmi
*/

#ifndef HEAD_H
#define HEAD_H

#define PATH_MSG "."
#define CHAR_MSG 'm'

#define P_GEN 1 // Definiamo solo i processi che spediscono

#define N 30 // Dopo quanti messaggi ricevuti bisogna fare la media

typedef struct {
	long type;
	double value;
} Message;

void generatore(int msgqid);
double mediatore(int msgqid);

#endif
