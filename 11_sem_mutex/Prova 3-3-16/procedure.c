#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "header.h"
#include <time.h>

void wait_sem(int id_sem, int numsem){
	struct sembuf sem_buf;

	sem_buf.sem_num = numsem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = -1;

	semop(id_sem, &sem_buf, 1);
}

void signal_sem(int id_sem, int numsem){
	struct sembuf sem_buf;

	sem_buf.sem_num = numsem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = 1;

	semop(id_sem, &sem_buf, 1);
}

void aggiornamento(Thunderstruct *ACDC, int id_sem){
	srand(time(0));
	int i,j;	
		
	wait_sem(id_sem, MUTEX);
	
	printf("Sto generando la matrice:\n");
	for(i = 0; i < ACDC->dim; i++){
		for(j = 0; j < ACDC->dim; j++){
			ACDC->mat[i][j] = rand() % 10;
		}
	}
	
	printf("Sto partorendo la matrice:\n");
	for(i = 0; i < ACDC->dim; i++){
		for(j = 0; j < ACDC->dim; j++){
			printf("%d ", ACDC->mat[i][j]);
		}
		printf("\n");
	}

	signal_sem(id_sem, MUTEX);
	signal_sem(id_sem, SYNCH);
}


void media(Thunderstruct *ACDC, int id_sem){
	int i,j;
	int somma;
	double media;
	somma = 0;
	media = 0;
	
	wait_sem(id_sem, SYNCH);	
	wait_sem(id_sem, MUTEX);
	
	printf("La matrice generata e': \n");
	for(i = 0; i < ACDC->dim; i++){
		for(j = 0; j < ACDC->dim; j++){
			//printf("%d ", ACDC->mat[i][j]);
			somma = somma + ACDC->mat[i][j];
		}
		//printf("\n");
	}
	media = (double) somma/((ACDC->dim)*(ACDC->dim));
	printf("La media e': %f\n", media);
	
	signal_sem(id_sem, MUTEX);
	signal_sem(id_sem, SYNCH);
}


void raddoppio(Thunderstruct *ACDC, int id_sem){
	int i,j;
	
	wait_sem(id_sem, SYNCH);
	wait_sem(id_sem, MUTEX);
	
	printf("La matrice raddoppiata è: \n");
	for(i = 0; i < ACDC->dim; i++){
		for(j = 0; j < ACDC->dim; j++){
			printf("%d ", 2*(ACDC->mat[i][j]));
		}
		printf("\n");
	}
	
	signal_sem(id_sem, MUTEX);
	signal_sem(id_sem, SYNCH);
}
