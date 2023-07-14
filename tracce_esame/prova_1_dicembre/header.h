/* 
Multithread (monitor) - Stack da implementare con i thread
*/

#ifndef HEADER_
#define HEADER_

#include <pthread.h>

typedef int Elem;

typedef struct {
	Elem *dati; // puntatore al vettore dinamico
	int dim; // dimensione dello stack
	
	int head; // puntatore alla cima dello stack
	pthread_cond_t is_not_full;
	pthread_cond_t is_not_empty;
	
	pthread_mutex_t mutex;
} Stack;

void stack_init(Stack*, int); // inizializza lo stack con una certa dimensione
void stack_remove(Stack*); // rimuove lo stack
void stack_push(Stack*, Elem); // pusha in cima allo stack
Elem stack_pop(Stack*); // rimuove dalla cima dello stack
int stack_size(Stack*); // grandezza attuale dello stack

#define NUM_PROD 5
#define NUM_CONS 1
#define NUM_CICLI_PROD 4
#define NUM_CICLI_CONS 10
void* prod(void*); // random fra 0 e 10 -> sleep(1) -> per 4 volte
void* cons(void*); // pop x 2 -> somma -> per 10 volte

#endif
