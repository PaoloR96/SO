#include <stdio.h> // printf
#include <stdlib.h> // NULL, sleep
#include <unistd.h> // ftok
#include <sys/types.h> // key_t
#include <sys/sem.h> // semctl, semget
#include <sys/shm.h> // shmctl, shmget
#include "header.h"

int main(){
	key_t semKey = ftok(PATH_SEM, CHAR_SEM);
	key_t shmKey = ftok(PATH_SHM, CHAR_SHM);

	int semID = semget(semKey, 6, 0664); // non creare se non esiste
	if(semID < 0){
		fprintf(stderr, "Semaphore not instantiated! Call main first\n"); // se non esisteva, dà errore
		return 1; // vogliamo che sia il main a generare tutte le strutture
	}
	int shmID = shmget(shmKey, sizeof(Structure), 0664);
	if(shmID < 0){
		fprintf(stderr, "Shared memory not istantiated! Call main first\n");
		return 2; // identico a sopra
	}

	Structure *sPtr;
	sPtr = (Structure*)shmat(shmID, 0, 0);
	if(sPtr == (void*)-1 ){
		fprintf(stderr, "Cannot attach shared memory to variable!\n");
		return 3;
	}

	while(1){
		if(sPtr->terminate) {
			return 0;
		}

		sleep(2);
		analizzatore(semID, sPtr);

		#ifdef DEBUG
		printf("PROCESSO %d: Analizzatore ha eseguito\n", getpid());
		#endif
	}
}
