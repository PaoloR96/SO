#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "monitor.h"
#include "lib.h"

//gestione sincronizzazione dei processi
void InizioConsumo(Monitor *monitor,Cond *conditions){

	printf("<%d> InizioConsumo \n", getpid());

	//faccio l'entrare il mio porcesso nel monitor e azzero il mutex in modo che sia l'unico processo presente
	enter_monitor(monitor);

	//adesso controllo se la variabile conditions->ok_consumo se è 0 significa che non c'è nulla da consumare
	//allora faccio la wait nel monitor, sul semaforo OK_CONSUMO dichiarato in precedenza e metto il
	//processo nella MONITOR WAITING AREA	
	if(conditions->ok_consumo == 0){
		wait_condition(monitor, OK_CONSUMO);
	}
}

void FineConsumo(Monitor *monitor, Cond *conditions){

	printf("<%d> fineConsumo \n", getpid());
	
	//ho terminato di consumare imposto la conditions->ok_consumo a 0 e conditions->ok_produzione a 1 
	conditions->ok_produzione = 1;
	conditions->ok_consumo = 0;

	//segnalo che ho terminato e avverto che devono produrre, al monitor sul semaforo OK_PRODUZIONE
	//in modo da attivare i produttori
	signal_condition(monitor, OK_PRODUZIONE);

	//abbandono monitor
	leave_monitor(monitor);	
}

void InizioProduzione(Monitor *monitor, Cond *conditions){

	printf("<%d> inizioProduzione \n", getpid());
	
	//faccio entrare processo nel monitor e azzero il mutex del monitor in modo da essere l'unico processo presente
	enter_monitor(monitor);	

	//controllo conditions->ok_produzione se 0 significa che non devo ancora produrre e aspettare 1, quindi attendo una signal
	// sul monitor da parte del consumatore su OK_PRODUZIONE
	if(conditions->ok_produzione == 0){
		wait_condition(monitor, OK_PRODUZIONE);
	}
}



void FineProduzione(Monitor *monitor, Cond *conditions){

	printf("<%d> fineProduzione \n", getpid());
	
	//finito di produrre imposto le variabili conditions->ok_produzione a 0 e conditions->ok_consumo a 1
	conditions->ok_produzione = 0;
	conditions->ok_consumo = 1;

	//seganlo che ho finito sul monitor e semafro OK_CONSUMO in modo da attivare i consumatori
	signal_condition(monitor, OK_CONSUMO);
	
	//abbandono monitor
	leave_monitor(monitor);
}

void consumatore(Monitor *monitor, Cond *conditions, Buffer *buffer){
	InizioConsumo(monitor, conditions);

	printf("<%d> valore consumato <%d>\n", getpid(), *buffer);	

	FineConsumo(monitor, conditions);
}

void produttore(Monitor *monitor, Cond *conditions, Buffer *buffer){
	InizioProduzione(monitor, conditions);
	*buffer = rand()%33;
	printf("<%d> valore prodotto <%d>\n ", getpid(), *buffer );
	FineProduzione(monitor, conditions);
}
