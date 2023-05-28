#include "header.h"
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void sem_op(int sem_id, int sem_num, int sem_val){
	struct sembuf s;
	s.sem_num = sem_num;
	s.sem_op = sem_val;
	s.sem_flg = 0;
	semop( sem_id, &s, 1);
}

void wait_sem(int sem_id, int sem_num){
	sem_op(sem_id, sem_num, -1);
}

void signal_sem(int sem_id, int sem_num){
	sem_op(sem_id, sem_num, +1);
}

unsigned int random_value(unsigned int min, unsigned int max){
	if( min == max )
		return min;
		
	if( min > max ){ // adjust things
		unsigned int temp = min;
		min = max;
		max = temp;
	}
	
	return rand() % (max-min+1) + min;
}

void generate_seeds(){
	struct timeval t;
	gettimeofday(&t, NULL);
	srand((unsigned)(t.tv_sec * t.tv_usec));
}

int cliente(Teatro *t){
	#ifndef SEEDS_ALREADY_GENERATED
	#define SEEDS_AREADY_GENERATED
	generate_seeds();
	#endif
	
	sleep( random_value(0,5) );
	
	short num_tentativi_prenotazione = random_value(1,4);
	
	for(int i = 0; i < num_tentativi_prenotazione; ++i){
		wait_sem(t->sem, MUTEX);
	
		if( t->disponibilita == 0 ){
			printf("[CLIENT] Spazio terminato!\n");
			signal_sem(t->sem, MUTEX);
			return -1;
		}
		
		int pos = -1;
		
		for(int i = 0; i < NUM_POSTI; ++i)
			if( t->vet[i].stato == LIBERO )
				pos = i;
				
		if( pos < 0 ){
			printf("[CLIENT] Spazio terminato, ma non l'ho rilevato con la verifica di disponibita!\n");
			signal_sem(t->sem, MUTEX);
			return -2;
		}
		
		t->disponibilita -= 1;
		t->vet[pos].stato = UPDATING;
		
		signal_sem(t->sem, MUTEX); // sblocco il mutex per altri clienti (o per il visualizzatore)
		// implementazione meno selfish, meno egoista, se sblocco il mutex dentro al for c'è la possibilità che qualcun altro esegua
		// un'altra soluzione sarebbe acquisire il mutex prima di entrare nel for e sbloccarlo una volta uscito
		
		sleep(1);
		
		t->vet[pos].id_cliente = getpid();
		
		t->vet[pos].stato = OCCUPATO;
	}	
	
	return 0;
}

void visualizzatore(Teatro *t){
	int exit = 0;
	
	while(!exit){
		wait_sem(t->sem, MUTEX);
		
		printf("\n-----\nDISPONIBILITA: %d\n", t->disponibilita);
		
		for(int i = 0; i < NUM_POSTI; ++i){
			switch( t->vet[i].stato ){
			case LIBERO:
				printf("POSTO %d: LIBERO\n", i);
				break;
				
			case OCCUPATO:
				printf("POSTO %d: OCCUPATO (id_cliente: %d)\n", i, t->vet[i].id_cliente);
				break;
				
			case UPDATING:
				printf("POSTO %d: IN AGGIORNAMENTO\n", i);
				break;
				
			default:
				break;
			}
		}
		
		if( t->disponibilita == 0 )
			exit = 1;
		
		signal_sem(t->sem, MUTEX);
		
		sleep(1);
	}
	
	return;
}
