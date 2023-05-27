# include <stdio.h>
#include <stdlib.h>

int main(){

	int somma ;
	int valore ;
	int i;
	
	printf("\n\n  ** Programma Somma di 4 elementi \n");
	printf("  ** Inserisco valori per calcolare la somma : \n");
	
	somma = 0;
	
	sleep(2);
	
	for(i =0; i<4; i++){
		
		sleep(2);
		valore = rand()%10;
		printf("  ** %d",valore);
		printf("\n");
		somma = somma + valore;
	}
	
	sleep(3);
	printf("\n\n  ** La somme e : %d",somma);
	printf("\n\n");

	return 0;
}
