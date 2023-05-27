#ifndef __HEADER__
#define __HEADER__

#define N 5

typedef struct {
	int vett[N];
	int sp;
}Pila;

void inizializza(Pila *);
void push(Pila *,int);
void stampa(Pila *);

#endif

