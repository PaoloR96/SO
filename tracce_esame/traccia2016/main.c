#include <stdio.h> // printf scanf
#include <stdlib.h> // exit, NULL, rand, srand
#include <time.h> // time
#include <unistd.h> // fork, execl
#include <wait.h> // wait
#include <sys/types.h> // pid_t
#include <sys/ipc.h> // IPC macros
#include <sys/sem.h> // semget, semctl
#include <sys/shm.h> // shmget, shmctl
#include <string.h> // strcmp
#include "header.h"

int main(int argc, char *argv[]){
	key_t semKey = ftok(PATH_SEM, CHAR_SEM);
	key_t shmKey = ftok(PATH_SHM, CHAR_SHM);
	
	int semID = semget(semKey, 6, IPC_CREAT|IPC_EXCL|0664);
	if(semID < 0){
		// la risorsa già esisteva, ci allacciamo ad essa e dopo la modifichiamo
		semID = semget(semKey, 6, 0664);
		if(semID < 0) {
			fprintf(stderr, "Cannot create sem structure!\n");
			return 1;
		}
	}
	
	int shmID = shmget(shmKey, sizeof(Structure), IPC_CREAT|IPC_EXCL|0664);
	if(shmID < 0){
		// la risorsa già esisteva, ci allacciamo ad essa e dopo la modifichiamo
		shmID = shmget(shmKey, sizeof(Structure), 0664);
		if(shmID < 0) {
			fprintf(stderr, "Cannot create shm structure!\n");
			return 2;
		}
	}
	
	/* inizializziamo i semafori */
	semctl(semID, MUTEX_WRITER_COUNT, SETVAL, 1);
	semctl(semID, MUTEX_READER_COUNT, SETVAL, 1);
	semctl(semID, CANWRITE, SETVAL, 0);
	semctl(semID, CANREAD, SETVAL, 1);
	semctl(semID, MUTEX_WRITE, SETVAL, 1);
	semctl(semID, MUTEX_PRINTF, SETVAL, 1);
	
	/* inizializziamo la shm */
	Structure *sPtr;
	sPtr = (Structure*)shmat(shmID, 0, 0);
	if(sPtr == (void*)-1){
		fprintf(stderr, "Cannot attach shm to variable!\n");
		return 3;
	}
	
	srand(time(NULL));
	type n = rand() % 3 + 3;
	if(argc > 1){
		if(!strcmp(argv[1],"3"))
			n = 3;
		if(!strcmp(argv[1],"4"))
			n = 4;
		if(!strcmp(argv[1],"5"))
			n = 5;
	}

	sPtr->n = n;
	sPtr->countReaders = 0;
	sPtr->countWriters = 0;
	sPtr->terminate = 0;
	
	for(int i = 0; i < sPtr->n; ++i)
		for(int j = 0; j < sPtr->n; ++j)
			sPtr->m[i][j] = 0;
	
	pid_t pid = fork();
	if(pid == 0){
		execl("./generatore", "./generatore", NULL);
	}
	
	pid = fork();
	if(pid == 0){
		execl("./elaboratore", "./elaboratore", NULL);
	}
	
	for(int i = 0; i < 2; ++i){
		pid = fork();
		if(pid == 0){
			execl("./analizzatore", "./analizzatore", NULL);
		}
	}
	
	sleep(15);
	sPtr->terminate = 1;
	
	int pidChild, st;
	for(int i = 0; i < 1 + 1 + 2; ++i){
		pidChild = wait(&st);
		#ifdef DEBUG
		printf("Figlio %d ha terminato con codice %d\n", pidChild, st>>8);
		#endif
	}
	
	/* dealloca le risorse */
	shmctl(shmID, IPC_RMID, 0);
	semctl(semID, 0, IPC_RMID);
}
