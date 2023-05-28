#include "header.h"
#include "../monitor/monitor.h"
#include <stdlib.h> // srand, rand, NULL
#include <sys/time.h> // gettimeofday, struct timeval
#include <stdio.h> // printf
#include <unistd.h> // sleep

void csmonitor_init(CSMonitor *cPtr){
	init_monitor( &cPtr->m, 3 );
	
	cPtr->job_id = -1;
	cPtr->is_buffer_full = 0;
	cPtr->count_client_fin = 0;
}

void csmonitor_destroy(CSMonitor *cPtr) {
	remove_monitor( &cPtr->m );
}

void produci_cliente_post(CSMonitor *cPtr){
	enter_monitor( &cPtr->m );
			
	while(1) {
		while( cPtr->is_buffer_full )
			wait_condition( &cPtr->m, BUFFER_EMPTY );
	
		while( cPtr->count_client_fin > 0 )
			wait_condition( &cPtr->m, CLIENT_FIN_OUTSIDE );
			
		if( !cPtr->is_buffer_full && cPtr->count_client_fin == 0 )
			break;
	}
	
	cPtr->is_buffer_full = 1;
	
	#ifndef SRAND_ALREADY_CALLED
	#define SRAND_ALREADY_CALLED
	struct timeval t;
	gettimeofday( &t, NULL );
	srand( (unsigned)(t.tv_usec * t.tv_sec ) );
	#endif
	
	cPtr->job_id = rand() % 50 + 51; // 0..49 -> 51..100
	
	printf( "[Cliente Post] Ho prodotto %d\n", cPtr->job_id );
	
	leave_monitor( &cPtr->m );
	
	signal_condition( &cPtr->m, BUFFER_FULL );
}

void produci_cliente_fin(CSMonitor *cPtr){
	enter_monitor( &cPtr->m );
	
	cPtr->count_client_fin += 1;
	
	while( cPtr->is_buffer_full )
		wait_condition( &cPtr->m, BUFFER_EMPTY );
	
	cPtr->is_buffer_full = 1;
	
	cPtr->count_client_fin -= 1;
	
	signal_condition( &cPtr->m, CLIENT_FIN_OUTSIDE );
	
	#ifndef SRAND_ALREADY_CALLED
	#define SRAND_ALREADY_CALLED
	struct timeval t;
	gettimeofday( &t, NULL );
	srand( (unsigned)(t.tv_usec * t.tv_sec ) );
	#endif
	
	cPtr->job_id = rand() % 50 + 1; // 0..49 -> 1..50
	
	printf( "[Cliente Fin] Ho prodotto %d\n", cPtr->job_id );
	
	leave_monitor( &cPtr->m );
	
	signal_condition( &cPtr->m, BUFFER_FULL );
}

void consuma_job(CSMonitor *cPtr){
	enter_monitor( &cPtr->m );
	
	while( !cPtr->is_buffer_full )
		wait_condition( &cPtr->m, BUFFER_FULL );
		
	int job_consumed = cPtr->job_id;
	
	printf( "[Sportello] Ho consumato %d\n", job_consumed );
	
	cPtr->is_buffer_full = 0;
	
	leave_monitor( &cPtr->m );
	
	signal_all( &cPtr->m, BUFFER_EMPTY );
}

void cliente_post(CSMonitor *cPtr){
	for(int i = 0; i < NUM_CICLI_POST; ++i){
		produci_cliente_post( cPtr );
		
		sleep(2);
	}
}

void cliente_fin(CSMonitor *cPtr){
	for(int i = 0; i < NUM_CICLI_FIN; ++i){
		produci_cliente_fin( cPtr );
		
		sleep(4);
	}
}

void sportellista(CSMonitor *cPtr){
	for(int i = 0; i < NUM_CICLI_SPORT; ++i){
		consuma_job( cPtr );
		
		sleep(1);
	}
}
