#include <pthread.h>
#include <stdio.h> // frprintf, printf
#include <stdlib.h> // exit, NULL
#include <unistd.h> // sleep

#include "header.h"

void stack_init(Stack* s, int dim){
	if( dim < 1 ){
		fprintf(stderr, "Cannot init stack with dim < 1!\n");
		exit(1);
	}
	
	s->dati = malloc( sizeof(Elem) * dim );
	s->dim = dim;
	
	s->head = 0;
	
	pthread_cond_init( &s->is_not_full, NULL );
	pthread_cond_init( &s->is_not_empty, NULL );
	
	pthread_mutex_init( &s->mutex, NULL );
}

void stack_remove(Stack* s){
	pthread_cond_destroy( &s->is_not_full );
	pthread_cond_destroy( &s->is_not_empty );
	
	pthread_mutex_destroy( &s->mutex );
	
	free( s->dati );
}

void stack_push(Stack* s, Elem e){
	pthread_mutex_lock( &s->mutex );
	
	while( s->head == s->dim )
		pthread_cond_wait( &s->is_not_full, &s->mutex );
	
	s->dati[s->head] = e;
	s->head += 1;
	
	pthread_cond_signal( &s->is_not_empty );
	
	pthread_mutex_unlock( &s->mutex );
}

Elem stack_pop(Stack* s){
	Elem to_return;
	
	pthread_mutex_lock( &s->mutex );
	
	while( s->head == 0 )
		pthread_cond_wait( &s->is_not_empty, &s->mutex );
		
	to_return = s->dati[s->head];
	s->head -= 1;
	
	pthread_cond_signal( &s->is_not_full );
	
	pthread_mutex_unlock( &s->mutex );
	
	return to_return;
}

int stack_size(Stack* s){
	pthread_mutex_lock( &s->mutex );
	
	int size = s->head;
	
	pthread_mutex_unlock( &s->mutex );
	
	return size;
}

void* prod(void* stackPtr){
	Stack *s = (Stack*)stackPtr;
	Elem to_put;
	
	for(int i = 0; i < NUM_CICLI_PROD; ++i){
		to_put = rand() % 10 + 1; // 0..9 -> 1..10
		
		stack_push( s, to_put );
		
		sleep(1);
	}
	
	pthread_exit(0);
}

void* cons(void* stackPtr){
	Stack *s = (Stack*)stackPtr;
	Elem op1, op2;
	
	for(int i = 0; i < NUM_CICLI_CONS; ++i){
		op1 = stack_pop( s );
		op2 = stack_pop( s );
		
		printf("op1 = %d, op2 = %d, op1 + op2 = %d\n", op1, op2, op1+op2 );
	}
	
	pthread_exit(0);
}
