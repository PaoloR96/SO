#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main(){

	//dichiarazione delle variabili
	key_t chiave_shm; 
	key_t chiave_sem;
	pid_t pid;
	Buffer *buffer;
	int id_sem;
	int id_shm;
	int status;
	int i;
	int k;

	//assegno le chiavi identiche al main 
	chiave_sem = ftok(".",'a');
	chiave_shm = ftok(".",'b');

	//mi vado a richiamare le strutture dati create
	id_shm = shmget(chiave_shm, 0, IPC_CREAT | 0664);
	id_sem = semget(chiave_sem, 0, IPC_CREAT | 0664);

	//faccio attach
	buffer = (Buffer*)shmat(id_shm, 0, 0);
	buffer->testa = 0;
	buffer->coda = 0;

	//procedo alla consumazione	
	for(i=0; i<2; i++){
		consumatore(id_sem, buffer);
	}
	return 0;
}
