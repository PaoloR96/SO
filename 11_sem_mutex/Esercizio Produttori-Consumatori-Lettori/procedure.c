#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"

void Signal_Sem(int id_sem, int num_sem){

	struct sembuf sem_buf;

	sem_buf.sem_num = num_sem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = 1;

	semop(id_sem,&sem_buf,1);

}

void Wait_Sem(int id_sem, int num_sem){

	printf("\t\tSto facendo la wait\n");

	struct sembuf sem_buf;
	
	sem_buf.sem_num = num_sem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = -1;

	semop(id_sem, &sem_buf, 1);

	printf("\t\tHo fatto la wait\n");

}

void Produzione(int semaphore, BufferCircolare* B){

	printf("\t\t\tTest1\n");

	Wait_Sem(semaphore, spazio_disponibile);

	printf("\t\t\tTest2\n");

	Wait_Sem(semaphore, MUTEXP);

	printf("\t\t\tTest3\n");
	
	printf("Produzione in corso...\n");
	B->elementi[B->testa] = 1 + rand()%10;
	printf("Ho prodotto: %d", B->elementi[B->testa]);
	(B->testa) = ++(B->testa) % 2;

	Signal_Sem(semaphore, MUTEXP);
	Signal_Sem(semaphore, messaggio_disponibile);

}

void Consumo(int semaphore, BufferCircolare *B){

	//Wait_Sem(semaphore, cella_disponibile);
	Wait_Sem(semaphore, messaggio_disponibile);
	Wait_Sem(semaphore, MUTEXC);

	printf("Consumo in corso...\n");
	printf("Ho consumato %d", B->elementi[B->coda]);
	//printf("Ho consumato %d", buffer[(*ptr_cella)]);
	(B->coda) = ++(B->coda) % 2;		

	Wait_Sem(semaphore, MUTEXC);
	Signal_Sem(semaphore, spazio_disponibile);
	//Signal_Sem(semaphore, messaggio_cella);

}

/*void Lettura(int semaphore, int* ptr_cella){

	Wait_Sem(semaphore, messaggio_cella);

	printf("Lettura in corso...\n");
	printf("Il valore nella cella è: %d", *ptr_cella);

	Signal_Sem(semaphore, cella_disponibile);

}*/
