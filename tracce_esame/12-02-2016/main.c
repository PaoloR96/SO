#include "header.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void generate_seeds();
unsigned int random_value(unsigned int min, unsigned int max);

typedef struct {
	Vettore *v1;
	Vettore *v2;

	short conteggio;
	double somma;

	pthread_mutex_t mutex;
} Buffer;

void* manager(void*);
void* worker(void*);

int main(){
	pthread_t threads[5]; // 1 manager, 4 worker
	pthread_attr_t attr;

	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

	Vettore *v1 = malloc(sizeof(Vettore));
	Vettore *v2 = malloc(sizeof(Vettore));

	inizializza(v1, 5);
	inizializza(v2, 5);

	Buffer *b = malloc(sizeof(Buffer));

	b->v1 = v1;
	b->v2 = v2;
	b->somma = 0;
	b->conteggio = 0;
	pthread_mutex_init( &b->mutex, NULL );

	for(int i = 0; i < 5; ++i){
		if( i == 0 )
			pthread_create( &threads[i], &attr, manager, (void*)b );
		else
			pthread_create( &threads[i], &attr, worker, (void*)b );
	}

	for(int i = 0; i < 5; ++i)
		pthread_join( threads[i], NULL );

	printf("Risultato del prodotto scalare: %lf\n", b->somma);

	distruggi(v1);
	distruggi(v2);
	free(v1);
	free(v2);

	pthread_mutex_destroy( &b->mutex );
	free(b);

	pthread_attr_destroy( &attr );

	pthread_exit(0);
}

void* manager(void* arg){
	Buffer *b = (Buffer*)arg;

	generate_seeds();

	for(int i = 0; i < 20; ++i){
		int elem = random_value(0, 5);

		if( i % 2 == 0 )
			inserisci_elemento(b->v1, elem);
		else
			inserisci_elemento(b->v2, elem);
	}

	pthread_exit( 0 );
}

void* worker(void* arg){
	Buffer *b = (Buffer*)arg;

	while(1){
		pthread_mutex_lock( &b->mutex );

		if( b->conteggio >= 10 ){
			pthread_mutex_unlock( &b->mutex );
			break;
		}
		else
			b->conteggio += 1;

		int val1 = preleva_elemento( b->v1 );
		int val2 = preleva_elemento( b->v2 );

		b->somma += (val1 * val2);

		printf( "[WORKER] Ho sommato %d. Ora somma vale %lf\n", (val1*val2), b->somma );

		pthread_mutex_unlock( &b->mutex );

		sleep(1);
	}

	pthread_exit( 0 );
}

void generate_seeds(){
	struct timeval t;
	gettimeofday( &t, NULL );

	srand( (unsigned)(t.tv_sec * t.tv_usec) );
}

unsigned int random_value(unsigned int min, unsigned int max){
	if( min == max )
		return min;

	if( min > max){ // set things right
		unsigned int temp = min;
		min = max;
		max = temp;
	}

	unsigned int distance = max - min + 1;

	return rand() % distance + min;
}


