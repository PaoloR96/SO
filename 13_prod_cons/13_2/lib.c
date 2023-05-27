#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib.h"


void WaitSem(int id_sem, int num_sem){
	struct sembuf sem_buf;
	sem_buf.sem_num = num_sem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = -1;
	semop(id_sem, &sem_buf,1);
}


void SignalSem(int id_sem, int num_sem){
	struct sembuf sem_buf;
	sem_buf.sem_num = num_sem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = 1;
	semop(id_sem, &sem_buf,1);
}

void Produttore(int id_sem, Buffer *buffer){

	WaitSem(id_sem, SPAZIO_DISP);
	WaitSem(id_sem, MUTEX_PROD);
	
	buffer->vettore[buffer->coda] = rand()%200;
	printf("Valore prodotto --%d--\n",buffer->vettore[buffer->coda]);
	buffer->coda =++ (buffer->coda)%DIMENSIONE;		

	SignalSem(id_sem, MUTEX_PROD);
	SignalSem(id_sem, MSG_DISP);

}

void Consumatore(int id_sem, Buffer *buffer){

	WaitSem(id_sem, MSG_DISP);
	WaitSem(id_sem, MUTEX_CONS);
	
	
	printf("leggo Messaggio in testa <%d>\n", buffer->vettore[buffer->testa]);
	buffer->vettore[buffer->testa] = 0;
	buffer->testa =++ (buffer->testa)%DIMENSIONE;			
	
	SignalSem(id_sem, MUTEX_CONS);
	SignalSem(id_sem, SPAZIO_DISP);

}
