#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/wait.h>
#include "header.h"

//#include "../monitor/monitor.h"

int main(){
	scaffale *vettscaffali;

	Monitor m;
	init_monitor(&m, NUM_CONDITION);

	int i, st, id_shm;
	pid_t pid, pid_ex;
	key_t chiave_shm, chiave_monitor;
	
	chiave_shm = ftok(PATH_SHM, CHAR_SHM);

	id_shm = shmget(chiave_shm, NUM_MAX*sizeof(scaffale), IPC_CREAT|0664);
	vettscaffali = (scaffale *)shmat(id_shm, 0, 0);
	
	for(i = 0; i < NUM_MAX; i++)
		vettscaffali[i].stato = libero;

	for(i = 0; i < NUM_PROCESSI; i++){
		pid = fork();
		
		if(pid == 0){
		
			if(i < NUM_FORNITORI){
				printf("Sono il fornitore numero %d con pid %d\n\n", i+1, getpid());
				produci(&m, vettscaffali);
			}
		
			else { 
				printf("Sono il cliente numero %d con pid %d\n\n", i-9, getpid());
				consuma(&m, vettscaffali);
			}
			exit(0);
		
		}
		
	}

	
	for(i = 0; i < NUM_PROCESSI; i++){
		if(pid == -1){
			perror("Errore nella creazione dei figli\n\n");
			_exit(-1);
		}
		
		else { 
			pid_ex = wait(&st);
			printf("Il figlio con pid %d ha terminato l'esecuzione con stato %d\n\n", pid_ex, st>>8);
		}
	}

	shmctl(id_shm, IPC_RMID, 0);
	remove_monitor(&m);
		

	return 0;
	
}
