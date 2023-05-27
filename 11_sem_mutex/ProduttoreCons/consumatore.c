#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>

#include "header.h"

int main(){

//Variabili utili
BufferCircolare* buf;
int shm_id, sem_id, k;

//Chiavi
key_t chiave_shm = ftok(PATH_SHM, CHAR_SHM);
key_t chiave_sem = ftok(PATH_SEM, CHAR_SEM);

//Inizializzazione shm e semaforo, a zero perché già dichiarate in start.c
shm_id = shmget(chiave_shm, 0, IPC_CREAT | 0664);
sem_id = semget(chiave_sem, 0, IPC_CREAT | 0664);

//Attach della memoria condivisa
buf = (BufferCircolare *) shmat(shm_id, 0, 0);

//Consumo
printf("[Consumatore] Sono il processo consumatore con pid %d, id della shared memory = %d, id dei semafori = %d\n", getpid(), shm_id, sem_id);
for(k = 0; k < 2; k++)
	consuma_elementi(sem_id, buf);

return 1;
}
