#ifndef __HEADER__
#define __HEADER__

#define N 100

typedef struct{
	int vett[N];
	int testa;
	int coda;
	int dimension;
	int dimensioneDisponibile;  
}Coda;

void push(Coda *,int);
int pop(Coda *);
void init(Coda *, int);
void top(Coda *);
void down(Coda *);
void stampa(Coda *);
bool isEmpty(Coda *);
bool isFull(Coda *);

#endif
