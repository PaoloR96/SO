#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "mialib.h"


void waitSem(int id_semaforo, int num_semaforo){
	struct sembuf sem_buf;
	
	sem_buf.sem_num = num_semaforo;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = -1;
	//semaforo rosso
	semop(id_semaforo, &sem_buf, 1); 
}

void signalSem(int id_semaforo, int num_semaforo){
	struct sembuf sem_buf;
	
	sem_buf.sem_num = num_semaforo;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = 1;
	//semaforo verse
	semop(id_semaforo, &sem_buf,1);
}

void usoRisorsa(msg* ptr_shm){
	
	printf("Il processo <%d> è nella sezione critica\n", getpid());
	int valore = ptr_shm->valore;
	sleep(1);
	valore++;
	ptr_shm->valore = valore;
	ptr_shm->modificatoDa=getpid();
}

