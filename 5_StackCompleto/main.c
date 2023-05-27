#include <stdio.h>
#include "mialib.h"
#include <iostream>
using namespace std;

int main(){

	Stack s;
	int scelta;
	int dimensione,valore,flag;
	
	flag = 0;
	while(flag == 0){
		printf("\n");
		printf("(1) iniziallizza lo stack \n");
		printf("(2) push \n");
		printf("(3) pop \n");
		printf("(4) top \n");
		printf("(5) stampa \n");
		printf("(0) per uscire \n\n");
		scanf("%d",&scelta);
		//while(scelta != 0){

			switch(scelta){
				case 1: 
				printf("\nQuanti elementi vuoi inserire : ");
				scanf("%d",&dimensione);
				init(&s,dimensione);
				printf("\n\n");
				break;
				
				case 2: 
				printf("\ninserisci un valore : ");
				scanf("%d",&valore);
				push(&s,valore);
				printf("\n\n");
				break;

				case 3: 
				printf("\n");
				valore = pop(&s);
				if(valore == -1){
					printf("\nlo stack è vuoto \n");
				}else{
					printf("ho estratto : ");
					printf("%4d",valore);
					printf("\n\n");
				}
				break;
				
				case 4: 
				printf("\nelemento in testa : ");
				top(&s);
				printf("\n\n");
				break;
				
				case 5: 
				printf("\nstampo lo stack : \n");
				stampa(&s);
				printf("\n\n");
				break;
				
				case 0: 
				flag = 1;
				break;
				
			
			}

	
		//}
	}
	
	
	return 0;

}
