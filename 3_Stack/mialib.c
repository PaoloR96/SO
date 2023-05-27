#include <stdio.h>
#include "mialib.h"

void inizzializza(Pila *p){
	p->sp = 0;
}


void push(Pila *p,int valore){
	p->vett[p->sp] = valore;
	p->sp++;
}

void stampa(Pila *p){
	int i = p->sp-1;
	printf("Pila : \n");
	while( i >= 0){
		printf("%4d",p->vett[i]);
		printf("\n");
		i--;
	}
}

