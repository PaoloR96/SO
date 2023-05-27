#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "lib.h"

//gestione semafori Mutua esclusione
void waitMutex(int id_sem, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = -1;
	semop(id_sem, &sem, 1);
}

void signalMutex(int id_sem, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = 1;
	semop(id_sem, &sem, 1);
}

//per sincronizzare produttore e consumatore
void waitSinc(int id_sem, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = -1;
	semop(id_sem, &sem, 1);
}

void signalSinc(int id_sem, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = 1;
	semop(id_sem, &sem, 1);
}



//gestione produttore consumatore
void produttore(int id_sem, Buffer *buffer){
			
	int i;
	
	//aspetto una signal dal consumatore che mi avverte che il buffer e vuoto e mi impoesso della risorsa
	waitSinc(id_sem, PRODUCI);
	waitMutex(id_sem, MUTEX);

	//produco dati fino a riempirla
	for(i=0; i<DIM; i++){
		buffer->vettore[i] = rand()%120;
		printf("Produco valore <%d> pid <%d>\n",buffer->vettore[i],getpid());
	}

	//libero la risorsa e segnalo al consumatore che adesso puo consumare		
	signalMutex(id_sem, MUTEX);
	signalSinc(id_sem, CONSUMA);		
}

void consumatore(int id_sem, Buffer *buffer){
	
	int i;

	//attendo che il produttore mi dia il segnale che posso consumare e mi impossesso della risorsa	
	waitSinc(id_sem, CONSUMA);
	waitMutex(id_sem, MUTEX);

	//leggo tutti i messaggi nel buffer e lo svuoto
	for(i=0; i<DIM; i++){
		printf("Messaggio letto <%d> pid <%d>\n",buffer->vettore[i],getpid());
		buffer->vettore[i] = 0;
	}

	//libero la risorsa e segnalo al prduttore che puo produrre	
	signalMutex(id_sem, MUTEX);
	signalSinc(id_sem, PRODUCI);		
}
