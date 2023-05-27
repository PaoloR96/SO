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

void produttore(shm *memoria){
	inizioProduzione( memoria );
	memoria->dato = rand()%33;
	printf("  # Dato prodotto <%d>, Produttore <%d>\n", memoria->dato, getpid());
	fineProduzione( memoria );
}

void consumatore(shm *memoria){
	inizioConsumazione( memoria );
	printf("  * Dato letto <%d>, Consumatore<%d>\n", memoria->dato, getpid());
	fineConsumazione( memoria );
}

void inizioProduzione(shm *memoria){
	//printf(" -- Entro nella coda di entrata del monitor, Produttore <%d>\n", getpid());
	enter_monitor( &(memoria->monitor) );
	if( memoria->ok_produci == 0 ){
		wait_condition( &(memoria->monitor), OK_PRODUCI);	
	}
	printf(">># Entro nel monitor, Produttore <%d>\n", getpid());
}

void fineProduzione(shm *memoria){
	printf("<<# Esco dal monitor, Produttore <%d>\n", getpid());
	memoria->ok_produci = 0;
	memoria->ok_consuma = 1;
	signal_condition( &(memoria->monitor), OK_CONSUMA);
	leave_monitor( &(memoria->monitor) );
}

void inizioConsumazione(shm *memoria){
	//printf(" -- Entro nella coda di entrata del monitor, Consumatore <%d>\n", getpid());
	enter_monitor( &(memoria->monitor) );
	if(memoria->ok_consuma == 0){
		wait_condition( &(memoria->monitor), OK_CONSUMA );	
	}
	printf(">>* Entro nel monitor, Consumatore <%d>\n", getpid());
}

void fineConsumazione(shm *memoria){
	printf("<<* Esco dal monitor, Consumatore <%d>\n", getpid());
	memoria->ok_consuma = 0;
	memoria->ok_produci = 1;
	signal_condition( &(memoria->monitor), OK_PRODUCI);
	leave_monitor( &(memoria->monitor) );
}
