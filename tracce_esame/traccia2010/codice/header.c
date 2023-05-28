#include "header.h"
#include "../monitor/monitor.h"

#include <stdlib.h> // rand
#include <stdio.h> // printf
#include <unistd.h> // getpid
#include <string.h> // strcpy

void inizioScrittura(Monitor *mPtr, Struttura *sPtr){
	enter_monitor(mPtr);

	if( sPtr->countReaders > 0 || sPtr->busy )
		wait_condition(mPtr, CAN_WRITE);

	sPtr->countWriters += 1;
	sPtr->busy = 1;

	leave_monitor(mPtr);
}

void fineScrittura(Monitor *mPtr, Struttura *sPtr){
	enter_monitor(mPtr);

	sPtr->busy = 0;
	sPtr->countWriters -= 1;

	if( sPtr->countWriters == 0 )
		signal_condition(mPtr, CAN_READ);

	leave_monitor(mPtr);
}

void scritturaMeteo(Monitor *mPtr, Struttura *sPtr){
	inizioScrittura(mPtr, sPtr);

	Meteo temp;

	temp.temperatura = rand() % 101 - 50;
	temp.umidita = rand() % 101;
	temp.pioggia = rand() % 2;

	sPtr->m = temp;

	printf("---PROCESSO %d HA SCRITTO---\n", getpid());

	fineScrittura(mPtr, sPtr);
}

void inizioLettura(Monitor *mPtr, Struttura *sPtr){
	enter_monitor(mPtr);

	if( sPtr->countWriters > 0 )
		wait_condition(mPtr, CAN_READ);

	sPtr->countReaders += 1;
	sPtr->busy = 1;

	signal_condition(mPtr, CAN_READ); // sblocco altri lettori

	leave_monitor(mPtr);
}

void fineLettura(Monitor *mPtr, Struttura *sPtr){
	enter_monitor(mPtr);

	sPtr->countReaders -= 1;

	if( sPtr->countReaders == 0 ){
		sPtr->busy = 0;
		signal_condition(mPtr, CAN_WRITE);
	}

	leave_monitor(mPtr);
}

void letturaMeteo(Monitor *mPtr, Struttura *sPtr) {
	inizioLettura(mPtr, sPtr);

	Meteo m = sPtr->m;
	char piove[] = "Piove";
	char nonPiove[] = "Non piove";
	char pioggia[10];
	if(m.pioggia == 1) strcpy(pioggia, piove);
	else strcpy(pioggia, nonPiove);

	printf("PROCESSO %d\nTemperatura: %d, umidita: %d%%. %s.\n", getpid(), m.temperatura, m.umidita, pioggia);

	fineLettura(mPtr, sPtr);
}

