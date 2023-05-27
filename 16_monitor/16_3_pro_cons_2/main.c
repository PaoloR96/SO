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

int main(){

	pid_t pid;
	shm *memoria;
	int id_shm;
	int status;
	int i,k;

	printf("\n\n __INIZIO__ \n\n");

	id_shm = shmget(IPC_PRIVATE, sizeof(shm), IPC_CREAT | 0664);
	memoria = (shm*)shmat(id_shm, 0, 0);
	memoria->ok_consuma = 0;
	memoria->ok_produci = 1;
	memoria->dato = 0;
	
	init_monitor ( &(memoria->monitor), NUM_CONDITIONS);

	for(i=0; i<NUM_PROC; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if( i%2 == 0){
				//Produttore
				printf("Sono PRODUTTORE <%d>\n",getpid());
				//for(k=0; k<2; k++)
				produttore(memoria);
			}else{
				//consumatore
				printf("Sono CONSUMATORE <%d>\n",getpid());
				//for(k=0; k<2; k++)
				consumatore(memoria);
			}
		
			exit(0);
		}
	}

	for(i=0; i<NUM_PROC; i++){
		pid = wait( &status);
		//printf("TERMINATO <%d> status <%d>\n", pid, status);
	}

	shmctl(id_shm, IPC_RMID, 0);
	remove_monitor( &(memoria->monitor) );

	printf("\n\n __FINE__ \n\n");

	return 0;
}
