/*	Problema Consumatore e Produttore risolto con Monitor	*/

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

int main(){

	//dichiarazione variabili
	int id_shm;
	int i;
	Buffer *buffer;		
	int status;
	pid_t pid;
	key_t chiave_shm;
	Monitor monitor;
	Cond *conditions;

	printf("\n\n __INIZIO__ \n\n");

	//assegno la chiave
	chiave_shm = IPC_PRIVATE;

	//Richiesta memoria condivisa, e attach 
	id_shm = shmget(chiave_shm, sizeof(Buffer)+sizeof(Cond), IPC_CREAT | 0664 );

	//la prima parte della memoria richieta e per il buffer	
	buffer = (Buffer*)shmat(id_shm, 0, 0);
	
	//la successiva parte e per la struct Cond
	conditions = (Cond*)(buffer+1);

	//inizializzo le variabili della struct Cond
	conditions->ok_consumo = 0;
	conditions->ok_produzione = 1;

	//inizializzo il monitor
	init_monitor(&monitor, NUM_CONDITIONS);

	//creo processi figli
	for(i=0; i<NUM_PROC; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if( i%2 == 0){
				//Produttore
				printf("Sono PRODUTTORE <%d>\n",getpid());
				sleep(3);
				produttore(&monitor, conditions, buffer);
				
			}else{
				//Consumatore
				printf("Sono CONSUMATORE <%d>\n",getpid());
				consumatore(&monitor, conditions, buffer);
			}
			exit(0);
		}
	}

	//attendo terminazione dei processi figli
	for(i=0; i<NUM_PROC; i++){
		pid = wait(&status);
		printf("processo <%d> TERMINATO con status <%d>\n", pid, status);
	}
	
	//rimuovo monitor e memoria condivisa
	remove_monitor(&monitor);
	shmctl(id_shm, IPC_RMID, 0);

	printf("\n\n __FINE__ \n\n");

	return 0;
}
