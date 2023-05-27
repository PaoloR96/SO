#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "lib.h"

//funzioni gestione semaforo
void signal(int id_sem, int num_sem){
	struct sembuf sem_buf;
	sem_buf.sem_num = num_sem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = 1;
	semop(id_sem, &sem_buf, 1);
}

void wait(int id_sem, int num_sem){
	struct sembuf sem_buf;
	sem_buf.sem_num = num_sem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = -1;
	semop(id_sem, &sem_buf, 1);
}

//funzioni gestione produttore e consumatore
void produttore(int id_sem, Pila*buffer){
	
	wait(id_sem, SPAZIO_DISP);
	wait(id_sem,MUTEX_PROD);
	
	//produco il dato e lo inserisco nella Pila
	buffer->vettore[buffer->testa] = rand()%123;
	printf("Produco dato <%d> inserito in testa [%d]\n",buffer->vettore[buffer->testa],buffer->testa);
	buffer->testa++;

	signal(id_sem, MUTEX_PROD);
	signal(id_sem, MSG_DISP);
}

void consumatore(int id_sem, Pila*buffer){
	wait(id_sem, MSG_DISP);	
	wait(id_sem,MUTEX_CONS);
	
	//prelevo il dato dalla Pila
	buffer->testa--;
	printf("Messaggi letto <%d> in testa [%d]\n",buffer->vettore[buffer->testa], buffer->testa);
	buffer->vettore[buffer->testa] = 0;

	signal(id_sem, MUTEX_CONS);
	signal(id_sem, SPAZIO_DISP);
}

