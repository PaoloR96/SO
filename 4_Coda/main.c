#include <stdio.h>
#include "mialib.h"

int main(){
	
	Coda coda;
	
	printf("inizializzo \n\n");
	inizializza(&coda);
	
	printf("inserisco valori \n\n");
	push(&coda,1);
	push(&coda,2);
	push(&coda,3);
	push(&coda,4);
	push(&coda,5);
	
	printf("stampo la coda \n\n");
	stampa(&coda);
	
	return 0;
}
