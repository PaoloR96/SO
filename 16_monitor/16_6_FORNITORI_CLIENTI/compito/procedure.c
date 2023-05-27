#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "header.h"
//#include "../monitor/monitor.h"

int livello_scorte = 0;

void produci(Monitor *m, scaffale *s){
	int i;

	for(i = 0; i < 15; i++){
		int indice = inizio_produzione(m, s);
		
		int pid = getpid();
		sleep(2);
		s[indice].id_fornitore = pid;
		printf("Ho prodotto il mio pid %d in posizione %d\n\n", pid, indice);
		
		fine_produzione(m, s, indice);

		sleep(1);
	}
}


int inizio_produzione(Monitor *m, scaffale *s){

	int indice = 0;
	enter_monitor(m);
	
	if(livello_scorte == NUM_MAX){
		wait_condition(m, SPAZIO_DISPONIBILE);
	}
	
	while(indice < NUM_MAX && s[indice].stato != libero){
		indice++;	
	}
	
	s[indice].stato = in_uso;

	leave_monitor(m);

	return indice;
}


void fine_produzione(Monitor *m, scaffale *s, int indice){
	enter_monitor(m);
	livello_scorte ++;
	s[indice].stato = occupato;
	
	signal_condition(m, MERCE_DISPONIBILE);	
	
	leave_monitor(m);
}


void consuma(Monitor *m, scaffale *s){
	int i;

	for(i = 0; i < 15; i++){
		int indice = inizio_consumo(m, s);

		int pid;
		sleep(2);
		pid = s[indice].id_fornitore;
		printf("Ho consumato il pid %d in posizione %d\n\n", pid, indice);

		fine_consumo(m, s, indice);

		sleep(1);
	}
}


int inizio_consumo(Monitor *m, scaffale *s){
	int indice = 0;
	enter_monitor(m);
	
	if(livello_scorte == NUM_MIN){
		wait_condition(m, MERCE_DISPONIBILE);
	}
	
	while(indice < NUM_MAX && s[indice].stato != occupato){
		indice++;	
	}
	
	s[indice].stato = in_uso;

	leave_monitor(m);

	return indice;
}


void fine_consumo(Monitor *m, scaffale *s, int indice){
	enter_monitor(m);
	livello_scorte --;
	s[indice].stato = libero;
	
	signal_condition(m, SPAZIO_DISPONIBILE);	
	
	leave_monitor(m);
}

