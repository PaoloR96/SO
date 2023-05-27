#ifndef __HRADER__
#define __HEADER__

#define N 5

typedef struct{

	int sp;			//stack pointer (testa)			
	int dimension;		//dmensione dello satck
	int vett[N];		//vettore dello stack

}Stack;


void init(Stack *,int);		//inizializza lo stack
bool isEmpty(Stack *);		//ci dice se lo stack è vuoto
bool isFull(Stack *);		//ci dice se lo stack è pieno
void push(Stack *, int);	//inserisce in testa un elemnto nello stack	
int pop(Stack *);		//estrae elemento in testa dallo stack
void stampa(Stack *);		//visualliza l'intero stack
void top(Stack *);        	//visualizzo elemento in teta

#endif
