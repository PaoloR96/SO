#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"

void init_gestione_voli(GestioneVoli *g){
	for(int i = 0; i < MAX_VOLI; ++i)
		g->vettore_stato[i] = LIBERO;
		
	g->num_voli = 0;
	
	pthread_mutex_init( &g->mutex_vettore_stato, NULL);
	pthread_mutex_init( &g->mutex_num_voli, NULL);
	
	pthread_cond_init( &g->spazio_disp, NULL);
	pthread_cond_init( &g->volo_disp, NULL);
}

void destroy_gestione_voli(GestioneVoli *g){
	pthread_mutex_destroy( &g->mutex_vettore_stato );
	pthread_mutex_destroy( &g->mutex_num_voli );
	
	pthread_cond_destroy( &g->spazio_disp );
	pthread_cond_destroy( &g->volo_disp );
}

int main(){
	pthread_t threads[NUM_THREAD];
	ThreadArg args[NUM_THREAD];
	GestioneVoli g;
	
	init_gestione_voli( &g );
	
	for(int i = 0; i < NUM_THREAD; ++i){
		args[i].gv = &g;
		args[i].index = i;
		
		pthread_create( &threads[i], NULL, doit, (void*)&args[i] );
	}
	
	for(int i = 0; i < NUM_THREAD; ++i)
		pthread_join( threads[i], NULL );
		
	destroy_gestione_voli( &g );
		
	pthread_exit(0);
}
