/*
 * mialic.c
 *
 *  Created on: 01 nov 2016
 *      Author: dome
 */

#include "mialib.h"
#include "elemento.h"
#include <stdio.h>

//setto la dimensione dello stack e il valore dello stack pointer (sp)
void inizzializza(Stack s, int dimensione){
	vett = (elemento*)malloc(sizeof(elemento)*dimensione);
	s.dim = dimensione;
	s.sp=0;
}

void push(Stack s, elemento e){

	//operazione di controllo per vedere se lo stack e pieno
	//se e vuoto inserisco
	s.vett[s.sp]= e;
	s.sp++;
}

void stampaStampaStack(Stack s){

	for(int i = s.sp; i>=0; i--){
		printf("stampo elemento del vettore",s.vett[i]);
		printf("\n");
	}
}

