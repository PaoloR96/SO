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

	//mi richiamo strutture dati create nel main
	id_shm = shmget(chiave_shm, 0, 0664);
	id_sem = semget(chiave_sem, 0, IPC_CREAT | 0664);

	//faccio attach
	buffer = (Buffer*)shmat(id_shm, 0, 0);
	buffer->testa = 0;
	buffer->coda = 0;

	//produco valore	
	k = 1+rand()%3;
	sleep(k);	

	srand(time(NULL)^getpid());
	
	//inserisco il valore (produttore)
	for(i=0; i<2; i++){
		produttore(id_sem, buffer);
	}

	return 0;
}
