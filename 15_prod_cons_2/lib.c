#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

//gestione semafori 
void wait_sem(int id_sem, int num_sem, int n){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = -n;
	semop(id_sem, &sem, 1); 	//semaforo rosso
}

void signal_sem(int id_sem, int num_sem, int n){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = n;
	semop(id_sem, &sem, 1); 	//semaforo verde
}

void produttore(int id_sem , Buffer *buffer){

	//attendo SPAZIO_DISP e MUTEX
	wait_sem(id_sem, SPAZIO_DISP, 1);
	wait_sem(id_sem, MUTEX, 1);

	//inserisco il valore	
	buffer->vettore[buffer->coda] = rand()%40;
	printf(">Dato prodotto [%d] coda <%d>\n", buffer->vettore[buffer->coda], buffer->coda);
	buffer->coda =++ (buffer->coda)%DIM;

	//rilascio MUTEX segnalo MSG_DISP
	signal_sem(id_sem, MUTEX, 1);
	signal_sem(id_sem, MSG_DISP, 1);
}

void consumatore(int id_sem, Buffer *buffer){

	//attendo MSG_DISP e lo azzero	
	wait_sem(id_sem, MSG_DISP, DIM);

	int i;
	//consumo tutto
	for(i=0; i<DIM; i++){
		printf(">Leggo dato [%d] testa <%d>\n",buffer->vettore[buffer->testa], buffer->testa);
		buffer->testa =++ (buffer->testa)%DIM;
	}

	//segano al produttore che ho terminato
	signal_sem(id_sem, SPAZIO_DISP, DIM);
}






