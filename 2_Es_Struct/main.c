/*
 * mian.c
 *
 *  Created on: 01 nov 2016
 *      Author: dome
 */

#include <stdio.h>
#include "mialib.h"

int main(){

	Stack stack;
	int dimensione;
	elemento e;


	printf("inserisci la dimensione dello stack : ");
	scanf(dimensione);
	inizzializza(stack,dimensione);

	printf("\n\n riempio con numeri random l'arrey");

	for(int i=0; i<dimensione; i++){
		e=rand()%100;
		printf("inserisco : ",e);
		printf("\n");
		push(stack,e);
	}

	printf("\n\n stampo \n\n");
	stampaStack(stack);

	return 0;
}

