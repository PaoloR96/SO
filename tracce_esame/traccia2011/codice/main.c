#include <stdio.h> // printf
#include <stdlib.h> // exit, srand, NULL
#include <time.h> // time
#include <wait.h> // wait
#include <unistd.h> // fork

#include <sys/shm.h>
#include <sys/ipc.h>

#include "header.h"
#include "../monitor/monitor.h"

int main() {
	Disco d; // variabile automatica
//	Disco *dPtr = malloc(sizeof(Disco));
//	Buffer *buffPtr = malloc(sizeof(Buffer)); // heap invece che shm
//	Monitor *mPtr = malloc(sizeof(Monitor)); // come sopra
	int bufferSHMID = shmget(IPC_PRIVATE, sizeof(Buffer), IPC_CREAT|0664);
	int monitorSHMID = shmget(IPC_PRIVATE, sizeof(Monitor), IPC_CREAT|0664);
	Buffer *bPtr = shmat(bufferSHMID, 0, 0);
	Monitor *mPtr = shmat(monitorSHMID, 0, 0);
	pid_t *dPtr = malloc(sizeof(pid_t) * SIZE_DISK);
	
	// init buffer
	initBuffer(bPtr);

	// init monitor
	init_monitor(mPtr, NUM_VAR);
	
	// init disk
	for(int i = 0; i < SIZE_DISK; ++i){
		dPtr[i] = 0;
	}

	for(int i = 0; i < NUM_SCHEDULER; ++i)
		if( fork() == 0 ) {
			schedulatore(mPtr, bPtr, dPtr);
			exit(0);
		}

	srand(time(NULL)); // generate real random numbers
	for(int i = 0; i < NUM_USERS; ++i)
		if( fork() == 0 ) {
			utente(mPtr, bPtr);
			exit(0);
		}
			
	for(int i = 0; i < NUM_SCHEDULER + NUM_USERS; ++i)
		wait(NULL);
		
	printf("\nCONTENUTO FINALE DEL DISCO:\n");
	for(int i = 0; i < SIZE_DISK; ++i){
		printf("[%d]: %d  ", i, dPtr[i]);
		
		if( i == SIZE_DISK / 2 ) printf("\n");
	}
	
	printf("\n\n");
	
//	free(buffPtr);
//	free(mPtr);
 	free(dPtr);
	shmctl(bufferSHMID, IPC_RMID, 0);
	shmctl(monitorSHMID, IPC_RMID, 0);
	remove_monitor(mPtr);
}
