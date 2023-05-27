#include <stdio.h>
#include "mialib.h"

//inizializza lo stack
void init(Stack *s,int dim){
	s->sp =0;
	s->dimension = dim;	
}

//ci dice se lo stack è vuoto		
bool isEmpty(Stack *s){
	if(s->sp == 0 ){return(true);}	
	else{return(false);}
		
}	

//ci dice se lo stack è pieno
bool isFull(Stack *s){
	if(s->sp == s->dimension){return(true);}	
	else{return(false);}
}	

//inserisce in testa un elemnto nello stack
void push(Stack *s, int valore){
	if(isFull(&*s)){			//se pieno restituisce il controllo al programma chiamante				
		printf("\nNon e possibile inserire, stack pieno\n");
		return ;
	}
	s->vett[s->sp]=valore;
	s->sp++;

}

//estrae elemento in testa dallo stack	
int pop(Stack *s){
	int valore;
	if(isEmpty(&*s)){			//se lo stack e vuoto restituisce al chiamante -1
		return -1;		
	}
	valore = s->vett[s->sp-1];
	s->sp--;
	printf("%4d",valore);
	printf("\n termino funzione \n");
	return valore;				//se ha fatto l'esatrazione restituisce il valore in testa allo stack
}	

//visualliza l'intero stack
void stampa(Stack *s){
	int i =0;
	i = s->sp-1;
	while(i>=0){
		printf("%4d",s->vett[i]);
		printf("\n");
	i--;	
	}	
}	

//visualizza l'elemento puntato dallo stack pointer	
void top(Stack *s){
	if(isEmpty(&*s)){
		printf("\nlo stack e vuoto \n");
	}else{
		printf("%4d",s->vett[s->sp-1]);
		printf("\n");
	}
} 
