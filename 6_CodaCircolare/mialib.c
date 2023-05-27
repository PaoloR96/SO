#include <stdio.h>
#include "mialib.h"


//ci restituisce true se la coda passata in inuput è vuota
bool isEmpty(Coda *c){

	if(c->dimensioneDisponibile == c->dimension){
		return true;
	}
	return false;	
}

//ci restituisce true se la coda passata in input è piena
bool isFull(Coda *c){

	if(c->dimensioneDisponibile == 0){
		return true;
	}
	return false;
}

//inserisce un elemnto nella coda in ultima posizione 
/*
 * si poteva sostiuire  c->coda++ e il seguente if con:
 * c->coda =++(c->coda % c->dimension);
 *
 */
void push(Coda *c, int valore){

	if(isFull(&*c)){
		printf(" coda piena \n");
		return ;
	}		
	
	//inserisco elemento in coda
	c->vett[c->coda] = valore;
	c->coda++;
	
	//incremento la coda e se una volta incrementata ha raggiunto la dimensione max la porto  0
	if(c->coda >= c->dimension){
		c->coda = 0;
	}						
	
	//decremento la dimensione disponibile della coda
	c->dimensioneDisponibile = c->dimensioneDisponibile -1;	
}

//preleva elemento che si trova in testa alla coda
int pop(Coda *c){
	
	int valore;
	if(isEmpty(&*c)){
		return -1;
	}
	
	//prelevo elemento dalla coda
	valore = c->vett[c->testa];
	c->vett[c->testa] = -1;
	c->testa ++;
	
	//incremento la testa e se una volta incrementata ha raggiunto la dimensione max la porto  0
	if(c->testa >= c->dimension){
		c->testa = 0;	
	}
	
	//incremento la dimensione disponibile della coda
	c->dimensioneDisponibile = c->dimensioneDisponibile +1;
	
	//restituisco il valore al programma chiamante
	return valore;	
}

//inizializzo la coda
void init(Coda *c, int dim){
	
	c->dimension = dim;
	c->dimensioneDisponibile = c->dimension;
	c->testa = 0;
	c->coda = 0;
	
	for(int i =0; i< dim ; i++){
		c->vett[i]=-1;	
	}

	stampa(c);	
}

//stampa a video l'elemento in testa (il prossimo a essere estratto)
void top(Coda *c){

	printf("\nelmento in testa -> : ");
	printf("%4d",c->vett[c->testa]);
	printf("\n");
}

//stampa l'elemento in coda (l'ultimo che è stato inserito)
void down(Coda *c){
	printf("\nultimo elmento -> : ");
	printf("%4d",c->vett[c->coda]);
	printf("\n");
}

//stampa l'array con i valori la testa e la coda a che posizione puntano e la quantità disponile di 
//elementi che puoi ancora inserire
void stampa(Coda *c){

	printf("\n| ");
	for(int i =0; i<c->dimension;i++){
		printf("%4d",c->vett[i]);
		printf(" | ");
	}
	
	printf("\n\n");
	printf("spazio ancora rimanete : ");
	printf("%4d",c->dimensioneDisponibile);
	printf("\n");
	printf("\ncoda -> : ");
	printf("%4d",c->coda);
	printf("\ntesta -> : ");
	printf("%4d",c->testa);
	printf("\n");
}	


