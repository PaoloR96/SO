#include "header.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int search(int stateVector[], int state){
	if(state != EMPTY && state != USING && state != FULL ) 
		return -1;
		
	for(int i = 0; i < SIZE_BUFF; ++i)
		if( stateVector[i] == state )
			return i;
			
	return -1;
}

void client(int server_queue_id){
	MessageCS m1;
	MessageSC m2;	
	m1.type = 1;
	m1.pid = getpid();
	
	m1.n1 = rand() % 11;
	m1.n2 = rand() % 11;
	m2.result = -1;

	msgsnd(server_queue_id, &m1, sizeof(m1) - sizeof(m1.type), 0);
	
	printf("PROCESSO %d: Ho mandato la coppia (%d,%d) al server\n", m1.pid, m1.n1, m1.n2);
		
	msgrcv(server_queue_id, &m2, sizeof(m2) - sizeof(m2.type), m1.pid, 0);
		
	printf("PROCESSO %d: Ho ottenuto la risposta %d dal server\n", m1.pid, m2.result);
}

void server(int server_queue_id){
	Buffer *b = malloc( sizeof( Buffer ) );
	
	pthread_mutex_init( &b->mutex, NULL );
	pthread_cond_init( &b->SPAZIO_DISP, NULL );
	pthread_cond_init( &b->MSG_DISP, NULL );
	
	b->server_qid = server_queue_id;
	for(int i = 0; i < SIZE_BUFF; ++i)
		b->state[i] = EMPTY;
	
	Manager *m = malloc( sizeof( Manager ) );
	m->bPtr = b;
	
	pthread_t threads[NUM_THREAD];
	pthread_t work_threads[NUM_THREAD_WORKER];

	for(int i = 0; i < NUM_THREAD_WORKER; ++i){
		pthread_create( &threads[i], NULL, worker, (void*)b );
		
		work_threads[i] = threads[i];
	}
	
	for(int i = NUM_THREAD_WORKER; i < NUM_THREAD; ++i){
		m->w_threads = work_threads;
		
		pthread_create( &threads[i], NULL, manager, (void*)m );
	}

	for(int i = 0; i < NUM_THREAD; ++i)
		pthread_join( threads[i], NULL );
	printf("SERVER: Faccio operazioni di pulizia e termino\n");
	
	// cleanup operations
	pthread_mutex_destroy( &b->mutex );
	pthread_cond_destroy( &b->SPAZIO_DISP );
	pthread_cond_destroy( &b->MSG_DISP );
	
	free( b );
	free( m );
}

void* manager(void* arg){
	Manager *mPtr = (Manager*)arg;
	Buffer *b = mPtr->bPtr;
	MessageCS m;
	
	while(1) {
		int r = msgrcv(b->server_qid, &m, sizeof(m) - sizeof(m.type), 0, IPC_NOWAIT );
		
		// se non ho ricevuto nulla, devo aspettare un secondo e ciclare ancora
		if( r < 0 ){
			sleep(1);
			continue;
		}
		
		// se ho ricevuto il codice di terminazione, devo chiudere tutto
		if( m.n1 == -1 && m.n2 == -1 ){
			printf("MANAGER: Segnale di terminazione ricevuto\n");
			
			for(int i = 0; i < NUM_THREAD_WORKER; ++i)
				pthread_cancel( mPtr->w_threads[i] );
				
			pthread_exit( NULL );
		}
		
		// metto l'elemento nel buffer con vettore di stato condiviso con i due thread worker
		
		int pos = -1;
		
		pthread_mutex_lock( &b->mutex );
		pos = search( b->state, EMPTY );
		
		while( pos < 0 ){
			pthread_cond_wait( &b->SPAZIO_DISP, &b->mutex );
			pos = search( b->state, EMPTY );
		}
		b->state[pos] = USING;
		pthread_mutex_unlock( &b->mutex );
		
		b->array[pos] = m;
		
		b->state[pos] = FULL;
		pthread_cond_signal( &b->MSG_DISP );
	}
}

void* worker(void* arg){
	Buffer *b = (Buffer*)arg;
	MessageCS from_buffer;
	MessageSC to_client;
	
	while(1) {
	
		// ottengo il primo elemento disponibile nel buffer circolare in maniera esclusiva
		int pos = -1;
			
		pthread_mutex_lock( &b->mutex );
		pos = search( b->state, FULL );
		
		while( pos < 0 ){
			pthread_cond_wait( &b->MSG_DISP, &b->mutex );
			pos = search( b->state, FULL );
		}
		b->state[pos] = USING;
		pthread_mutex_unlock( &b->mutex );
	
		from_buffer = b->array[pos];
	
		b->state[pos] = EMPTY;
		pthread_cond_signal( &b->SPAZIO_DISP );
	
		// ho ottenuto il messaggio CS (rcv), quindi popolo quello SC (snd) di conseguenza e lo mando

		to_client.type = from_buffer.pid; // lo faccio ricevere al processo con quel pid	
		to_client.result = from_buffer.n1 * from_buffer.n2;
	
		msgsnd( b->server_qid, &to_client, sizeof(to_client) - sizeof(to_client.type), 0 );
	}
}
