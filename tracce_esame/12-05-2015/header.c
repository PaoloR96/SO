#include "header.h"
#include <pthread.h>
#include <stdlib.h> // exit
#include <stdio.h> // printf
#include <unistd.h> // sleep
#include <sys/time.h> // struct timeval, gettimeofday

void inserisci_volo(GestioneVoli *g, int id){
	pthread_mutex_lock( &g->mutex_num_voli );
	
	while( g->num_voli == MAX_VOLI )
		pthread_cond_wait( &g->spazio_disp, &g->mutex_num_voli ); // bloccante
	
	g->num_voli += 1;
	
	pthread_mutex_unlock( &g->mutex_num_voli );
	
	pthread_mutex_lock( &g->mutex_vettore_stato );
	
	int pos = -1;
	
	for(int i = 0; i < MAX_VOLI; ++i)
		if( g->vettore_stato[i] == LIBERO ){
			pos = i;
			break;
		}
		
	if( pos < 0 ){
		printf("[P] Fatal error in inserisci_volo: Free position not found\n");
		exit(1);
	}
	
	g->vettore_stato[pos] = IN_USO;
	
	pthread_mutex_unlock( &g->mutex_vettore_stato );
	
	Volo v;
	v.id = id;
	v.quota = 0;
	
	g->vettore_voli[pos] = v;
	
	g->vettore_stato[pos] = OCCUPATO;
	
	sleep(1);
	
	printf("[P] Creato volo (id: %d) in posizione %d\n", v.id, pos);
	
	// pthread_cond_signal( &g->volo_disp );
	// inutile, poiché rimuovi_volo non fa la wait su volo_disp
}

void rimuovi_volo(GestioneVoli *g, int id){
	while(1) { // polling
		usleep(1000); // attendi 1ms per permettere ad altri processi di accedere alla var_cond e modificarla
		
		pthread_mutex_lock( &g->mutex_num_voli );
		
		if( g->num_voli == 0 ){
			// invece di fare la wait_cond, sblocco il mutex e ripeto il ciclo
			pthread_mutex_unlock( &g->mutex_num_voli );
		} else {
			g->num_voli -= 1;
			pthread_mutex_unlock( &g->mutex_num_voli);
			break; // esci dal while
		}
	}
	
	pthread_mutex_lock( &g->mutex_vettore_stato );
	
	int pos = -1;
	
	for(int i = 0; i < MAX_VOLI; ++i)
		if( g->vettore_stato[i] == OCCUPATO )
			if( g->vettore_voli[i].id == id ){
				pos = i;
				break;
			}
	
	if( pos < 0 ){
		printf("[C] Fatal error in rimuovi_volo: Occupied position not found\n");
		exit(1);
	}
	
	g->vettore_stato[pos] = IN_USO;
	
	pthread_mutex_unlock( &g->mutex_vettore_stato );
	
	Volo v = g->vettore_voli[pos];
	
	g->vettore_stato[pos] = LIBERO;
	
	sleep(1);
	
	printf("[C] Ho eliminato il volo (id: %d) in posizione %d\n", v.id, pos);
	
	pthread_cond_signal( &g->spazio_disp );
}

void aggiorna_volo(GestioneVoli *g, int id, int new_quota){
	int pos = -1;
	
	while( pos < 0 ) { // polling		
		pthread_mutex_lock( &g->mutex_vettore_stato );
		
		for(int i = 0; i < MAX_VOLI; ++i)
			if( g->vettore_stato[i] == OCCUPATO )
				if( g->vettore_voli[i].id == id ){
					pos = i;
					break;
				}

		if( pos >= 0 )
			g->vettore_stato[pos] = IN_USO;
				
		pthread_mutex_unlock( &g->mutex_vettore_stato );
	}
	
	Volo v = g->vettore_voli[pos];
	
	g->vettore_voli[pos].quota = new_quota;
	
	g->vettore_stato[pos] = OCCUPATO;
	
	#ifndef SRAND_ALREADY_CALLED
	#define SRAND_ALREADY_CALLED
	struct timeval tv;
	gettimeofday( &tv, NULL );
	srand( (unsigned)( tv.tv_usec * tv.tv_sec ) );
	#endif // SRAND_ALREADY_CALLED
	
	int time_to_sleep = rand() % 3 + 1; // 1, 2, 3
	
	sleep(time_to_sleep);
	
	printf("[A] Ho aggiornato il volo (id: %d), da quota %d a quota %d\n", v.id, v.quota, new_quota);
}

void* doit(void* arg){
	ThreadArg *t = (ThreadArg*)arg;
	
	if( t->index < 0 ){
		printf("[T] Fatal error in doit: index lesser than zero\n");
		exit(1);
	}
	
	inserisci_volo(t->gv, t->index);
	
	aggiorna_volo(t->gv, t->index, 1000);
	aggiorna_volo(t->gv, t->index, 2000);
	aggiorna_volo(t->gv, t->index, 1000);
	
	rimuovi_volo(t->gv, t->index);
	
	pthread_exit(0);
}

