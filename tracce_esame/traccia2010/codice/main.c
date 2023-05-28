#include <stdio.h> // printf
#include <stdlib.h> // NULL
#include <unistd.h> // fork
#include <wait.h> // wait
#include <sys/types.h> // pid_t
#include <sys/shm.h> // shmget, shmctl
#include <sys/ipc.h> // IPC macros
#include <time.h> // time

#include "header.h"
int main(){
	int shmID = shmget(IPC_PRIVATE, sizeof(Struttura), IPC_CREAT|IPC_EXCL|0664);
	if(shmID < 0){
		// già esisteva la struttura, allora la elimino
		shmID = shmget(IPC_PRIVATE, sizeof(Struttura), 0664);
		shmctl(shmID, IPC_RMID, 0);
		shmID = shmget(IPC_PRIVATE, sizeof(Struttura), IPC_CREAT|0664);
		if(shmID < 0){
			fprintf(stderr, "Cannot get shm from OS\n");
			return 1;
		}
	}

	Monitor monitor;
	init_monitor(&monitor, NUM_VAR);

	Struttura *sPtr;
	sPtr = (Struttura*)shmat(shmID, 0, 0);
	sPtr->countReaders = 0;
	sPtr->countWriters = 0;
	sPtr->busy = 0;

	if(sPtr == (void*)-1){
		fprintf(stderr, "Cannot attach shm to variable\n");
		return 2;
	}

	srand(time(NULL));

	for(int i = 0; i < NUM_LETTORI; ++i){
		pid_t pid = fork();
		if(pid == 0){
			for(int j = 0; j < NUM_LETTURE; ++j){
				#ifndef FAST_
				sleep(1);
				#endif
				letturaMeteo(&monitor, sPtr);
			}
			return 0;
		}
	}

	pid_t pid2 = fork();
	if(pid2 == 0){
		for(int i = 0; i < NUM_SCRITTURE; ++i){
			#ifndef FAST_
			sleep(2);
			#endif
			scritturaMeteo(&monitor, sPtr);
		}
		return 0;
	}

	for(int i = 0; i < NUM_PROCESSI; ++i)
		wait(NULL);

	shmctl(shmID, IPC_RMID, 0);
}
