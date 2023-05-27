#include <stdlib.h>
#include <stdio.h>

int main(void){
	
	int *ptr;
	int dim = 4;
	int i ;
	
	//sto allozando una dimensione dinamicamente e usando un puntatore
	ptr =(int*)malloc((sizeof(int))*dim);
	
	//riempio array 
	for(i = 0; i < dim ; i++){
		ptr[i] = -1;
	}
	
	printf("\n\n");
	
	//stampo
	for(i=0; i < dim ; i++){
		printf("%d\n",ptr[i]);
	}
	
	//metto un valore nella posizione 3 del vettore
	ptr[3] = 4;
	
	printf("\n\n");
	
	//stampo
	for(i=0; i < dim ; i++){
		printf("%d\n",ptr[i]);
	}

 
	printf("\n\nTerminato\n\n");
	
	return 0;
}
