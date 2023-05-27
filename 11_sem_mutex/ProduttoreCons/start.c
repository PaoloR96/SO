/*
SEMAFORI:

1. MUTEXP (da 1)
2. spazio_disponibile (che parte da N)
3. numero_messaggi (che parte da 0)

SHARED MEMORY: si passa il BufferCircolare (di grandezza sizeof(BufferCircolare))

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>

#include "header.h"

#define N_PROC 11

int main(){

//Variabili utili
int st, i, sem_id, shm_id;
pid_t pid, pid_ex;

//Chiavi della memoria condivisa
key_t chiave_sem = ftok(PATH_SEM, CHAR_SEM);
key_t chiave_shm = ftok(PATH_SHM, CHAR_SHM);

//Creazione semafori e memoria condivisa
sem_id = semget(chiave_sem, 3, IPC_CREAT | 0664);
shm_id = shmget(chiave_shm, sizeof(BufferCircolare), IPC_CREAT | 0664);

//Inizializzazione dei semafori
semctl(sem_id, MUTEXP, SETVAL, 1);
semctl(sem_id, SPAZIO_DISP, SETVAL, N);
semctl(sem_id, MESS_DISP, SETVAL, 0);

//Non si fa l'attach della shmem perché viene fatto in produttore e consumatore

//fork()

for(i = 0; i < N_PROC; i++){
	
	pid = fork();

	if(pid == 0){
	
		if(i == 0){
			//CONSUMATORE
			execl("./consumatore", "./consumatore", NULL);
		
		} else {
			//PRODUTTORI
			execl("./produttore", "./produttore", NULL);			
	
		}
		_exit(0);

	}
	sleep(1 + rand()%3);

}

for(i = 0; i < N_PROC; i++){

	if(pid == -1){
		perror("ERRORE - fork() non riuscita.\n");
		_exit(1);

	} else {
		//MASTER
		pid_ex = wait(&st);
		printf("[MASTER] - Il figlio numero %d è terminato con stato %d\n", pid_ex, st>>8);
	}
}

//Cancellazione della shared memory e dei semafori
shmctl(shm_id, IPC_RMID, 0);
semctl(sem_id, 0, IPC_RMID);

return 0;
}
