#ifndef __HEADER__
#define __HEADER__

#define N 5

typedef struct{
	int vett[N];
	int testa;
	int coda;
}Coda;

void inizializza(Coda *);
void push(Coda *, int);
void stampa(Coda *);

#endif
