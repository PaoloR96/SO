#include "lib.h"

int main(){
	
	//variabili
	monitor_treno *m;
	pthread_t threads[NUM_THREADS+10];
	pthread_attr_t attr;
	int i;
	
	printf("\n\n _INIZIO_ \n\n");
	
	//alloco memoria
	m = (monitor_treno*)malloc(4*sizeof(monitor_treno));

	//inizializzo lestrutture create
	inizializza(m);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	//genero i threads
	for(i=0; i<NUM_THREADS+10; i++){
		if(i < NUM_THREADS){
			//capitreno
			//indice di riferimento al treno
			m[i].i = i;
			pthread_create(&threads[i], &attr, capo_treno ,(void*)&m[i]);
		}
		else{
			//viagiatori
			pthread_create(&threads[i], &attr, viaggiatori,(void*)m);
		}
	}
	
	//attendo che temrinano i threads
	for(i=0; i<NUM_THREADS+10; i++){
		pthread_join(threads[i], NULL);
	}
	
	//rmuovo le strutture create
	rimuovi(m);
	
	printf("\n\n _FINE_ \n\n");
	
	pthread_exit(0);
}
