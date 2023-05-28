#include "header.h"
#include "monitor.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

short __num_accessi_fornitore = 15;
short __num_accessi_cliente = 15;

void single_fornitore(Magazzino *maPtr, Monitor *moPtr){
	enter_monitor(moPtr);

	while( maPtr->livello_scorte == SIZE_MAGAZZINO )
		wait_condition(moPtr, OK_FORNITORE);

	int pos = -1;

	for(int i = 0; i < SIZE_MAGAZZINO; ++i)
		if( maPtr->s[i].stato == STATO_LIBERO )
			pos = i;

	if( pos < 0 ){
		fprintf( stderr, "FATAL ERROR! Trespassed wait_cond but cannot find empty pos\n");
		exit(1);
	}

	maPtr->s[pos].stato = STATO_INUSO;

	leave_monitor(moPtr);

	maPtr->s[pos].id_fornitore = getpid();

	#ifndef FAST
	sleep(2); // simula la produzione
	#endif

	maPtr->s[pos].stato = STATO_OCCUPATO;

	enter_monitor(moPtr);

	maPtr->livello_scorte += 1;

	printf("[F] Alla posizione %d ho prodotto %d, e ora livello_scorte vale %d \n", pos, maPtr->s[pos].id_fornitore, maPtr->livello_scorte );

	signal_condition(moPtr, OK_CLIENTE);

	leave_monitor(moPtr);
}

void single_cliente(Magazzino *maPtr, Monitor *moPtr){

	enter_monitor(moPtr);

	while( maPtr->livello_scorte == 0 )
		wait_condition(moPtr, OK_CLIENTE);

	int pos = -1;

	for(int i = 0; i < SIZE_MAGAZZINO; ++i)
		if( maPtr->s[i].stato == STATO_OCCUPATO )
			pos = i;

	if( pos < 0 ){
		fprintf( stderr, "FATAL ERROR! Trespassed wait_cond but cannot find full pos\n");
		exit(1);
	}

	maPtr->s[pos].stato = STATO_INUSO;

	leave_monitor(moPtr);

	maPtr->s[pos].id_fornitore = 0;

	#ifndef FAST
	sleep(2); // simula l'acquisto
	#endif

	maPtr->s[pos].stato = STATO_LIBERO;

	enter_monitor(moPtr);

	maPtr->livello_scorte -= 1;

	printf("[C] Alla posizione %d ho consumato, e ora livello_scorte vale %d \n", pos, maPtr->livello_scorte );

	signal_condition(moPtr, OK_FORNITORE);

	leave_monitor(moPtr);
}

void fornitore(Magazzino *maPtr, Monitor *moPtr){
	for(int i = 0; i < __num_accessi_fornitore; ++i){
		single_fornitore(maPtr, moPtr);

		#ifndef FAST
		sleep(1);
		#endif
	}
}

void cliente(Magazzino *maPtr, Monitor *moPtr){
	for(int i = 0; i < __num_accessi_cliente; ++i){
		single_cliente(maPtr, moPtr);

		#ifndef FAST
		sleep(1);
		#endif
	}
}

void init_magazzino(Magazzino *maPtr){
	for(int i = 0; i < SIZE_MAGAZZINO; ++i)
		maPtr->s[i].stato = STATO_LIBERO;

	maPtr->livello_scorte = 0;
}
