#include <stdio.h>
#include "mialib.h"

void inizializza(Coda *c){
	c->testa = 0;
	c->coda = c->testa;
}

void push(Coda *c, int valore){
	
	c->vett[c->coda] = valore;
	c->coda++;
}

void stampa(Coda *c){

	for(int i=c->testa;i<c->coda;i++){
	
		printf("%4d",c->vett[i]);
		printf("\n");
	}
}	

