#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "./Monitor/monitor.h"
#include "lib.h"

void Lettore(MonitorLettScritt *shm){

	InizioLettura(shm);
	
	//leggo il dato
	printf(">> Leggo il dato [%d] lettori presenti <%d>, processo <%d>\n", shm->dato, shm->num_lettori, getpid());

	
	FineLettura(shm);
}

void Scrittore(MonitorLettScritt *shm){

	InizioScrittura(shm);

	//produco il dato
	shm->dato = rand()%33;
	printf("> Dato prodotto [%d] da processo <%d>\n", shm->dato, getpid() );	

	FineScrittura(shm);
}

void InizioLettura(MonitorLettScritt *shm){

	//faccio entrare il processo nel monitor
	enter_monitor( &(shm->monitor) );

	//constrollo se ci sono scrittori nel monitor quindi shm->ocupato e 
	//non devono esserci scritori shm->num_scrittori == 0, allora attendo la SYNCH_LETTORI 
	//da parte degli scirttori (signal(SYNCH_LETTORI) ).
	if((shm->occupato == 1) && (shm->num_lettori == 0)){
		wait_condition( &(shm->monitor), SYNCH_LETTORI );	
	}

	//sono sicuro di essere il solo lettore all'interno del monitor, cambio valore
	//a shm->occupato, incremento num_lettori di 1
	shm->occupato = 1;
	shm->num_lettori ++;

	//sblocco gli altri lettori e abbandono il monitor per permettere agli altri lettori di entrare
	signal_condition( &(shm->monitor), SYNCH_LETTORI);
	leave_monitor ( &(shm->monitor) );	
}

void InizioScrittura(MonitorLettScritt *shm){

	//entro nel monitor
	enter_monitor( &(shm->monitor) );

	//controllo che non ci sia nessuno dentro, se è occupato aspetto una sgnal(SYNCH_SCRITTORI)
	if(shm->occupato == 1) {
		wait_condition( &(shm->monitor), SYNCH_SCRITTORI);
	}

	//metto in occupato
	shm->occupato = 1;

	//abbandono il monitor
	//leave_monitor( &(shm->monitor) );
}

void FineLettura(MonitorLettScritt *shm){

	//entro nuovamente nel monitor
	enter_monitor( &(shm->monitor) );

	//decremento il numero di lettori shm->num_lettori e metto shm->occupato = 0
	shm->num_lettori --;

	//se non ci sono piu lettori nel monitor, shm->occupato = 0 e passo il controllo agli scittori
	if(shm->num_lettori == 0){
		shm->occupato = 0;
		signal_condition( &(shm->monitor), SYNCH_SCRITTORI );
	}
	
	//abbandono il monitor
	leave_monitor( &(shm->monitor) );
}

void FineScrittura(MonitorLettScritt *shm){

	//entro nel monitor
	enter_monitor( &(shm->monitor) );

	//iposto occupato a 0
	shm->occupato = 0;

	//sblocco gli altri scittori in coda
	if(queue_condition( &(shm->monitor), SYNCH_SCRITTORI) > 0 ){
		signal_condition( &(shm->monitor), SYNCH_SCRITTORI);
	}

	//sblocco gli altri lettori in coda
	if(queue_condition( &(shm->monitor), SYNCH_LETTORI) > 0){
		signal_condition( &(shm->monitor), SYNCH_LETTORI);
	}
	
	//esco dal monitor
	leave_monitor( &(shm->monitor) );
}
