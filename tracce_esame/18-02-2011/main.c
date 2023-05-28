#include "header.h"
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <stdio.h>

int main(){
	// Uso delle chiavi per non sporcare la memoria in caso di mancata rimozione
	key_t key_sem = ftok(".", 'A');
	key_t key_shm = ftok(".", 'S');
	
	// Istanzio il semaforo
	int sem_id = semget(key_sem, 1, IPC_CREAT|IPC_EXCL|0664);
	if(sem_id < 0){
		sem_id = semget(key_sem, 1, 0664);
		if(sem_id < 0){
			printf("Cannot parse sem from sys!\n");
			return -1;
		}
	}
	
	// Istanzio la shm
	int shm_id = shmget(key_shm, sizeof(Teatro), IPC_CREAT|IPC_EXCL|0664);
	if(shm_id < 0){
		shm_id = shmget(key_shm, sizeof(Teatro), 0664);
		if(shm_id < 0){
			printf("Cannot parse shm from sys!\n");
			return -1;
		}
	}
	
	// Init delle strutture dati
	semctl(sem_id, MUTEX, SETVAL, 1);
	
	Teatro *t = (Teatro*)shmat(shm_id, 0, 0);
	
	t->sem = sem_id;
	t->disponibilita = NUM_POSTI;
	
	for(int i = 0; i < NUM_POSTI; ++i)
		t->vet[i].stato = LIBERO;
		
	// Generazione figli
	pid_t pid;
	
	for(int i = 0; i < NUM_CLIENTI + 1; ++i){
		pid = fork();
		
		if(pid == 0){
			if( i == 0 ){
				visualizzatore(t);
			} else {
				cliente(t);
			}
			
			exit(0);
		}
	}
	
	// Attesa figli
	for(int i = 0; i < NUM_CLIENTI + 1; ++i)
		wait(NULL);
	
	// Rimozione strutture dati kernel
	shmdt(t);
	shmctl(shm_id, IPC_RMID, NULL);
	semctl(sem_id, 0, IPC_RMID);
}
