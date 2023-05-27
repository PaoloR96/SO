#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include "header.h"
#include <math.h>

void Wait_Sem(int id_sem, int numsem, int n)     {
	struct sembuf sem_buf;

	sem_buf.sem_num = numsem;
	sem_buf.sem_op = -n;
	sem_buf.sem_flg = 0;

	semop(id_sem, &sem_buf, 1);

}


void Signal_Sem (int id_sem, int numsem, int n)     {
	struct sembuf sem_buf;

	sem_buf.sem_num = numsem;
	sem_buf.sem_op = n;
	sem_buf.sem_flg = 0;

	semop(id_sem, &sem_buf, 1);

}


double media(int* vett){
	double somma = 0;
	int i;

	for(i = 0; i < N; i++)
		somma = somma + (vett[i]);

	return somma/N;	

}

double varianza(int* vett, double media){
	double somma = 0;	
	int i;

	for(i = 0; i < N; i++)
		somma = somma + (vett[i] - media)*(vett[i] - media);

	return sqrt(somma/(N-1));
}

void produci_elemento(int sem_id, BufferCircolare* buf){
	int val;
	
	val = (1 + rand()%10);

	//Semafori della morte
	Wait_Sem(sem_id, SPAZIO_DISP, 1);
	Wait_Sem(sem_id, MUTEXP, 1);

	printf("Produzione elemento...\n");
	buf->elementi[buf->testa] = val;
	printf("[Produttore %d] ho prodotto %d in posizione %d\n", getpid(), val, buf->testa);
	buf->testa = ++(buf->testa) % N;

	//Altri semafori della morte
	Signal_Sem(sem_id, MUTEXP, 1);
	Signal_Sem(sem_id, MESS_DISP, 1);

}

void consuma_elementi(int sem_id, BufferCircolare* buf){
	int i;
	double media1, varianza1;
	int vettore_elementi[N];

	//Semaforo
	Wait_Sem(sem_id, MESS_DISP, 5);

	for(i = 0; i < N; i++)
		vettore_elementi[i] = buf->elementi[i];
	
	buf->testa = 0;

	printf("[Consumatore %d] consumo elementi\n",getpid());
	
	media1 = media(vettore_elementi);
	printf("La media è: %f.\n", media1);
	
	varianza1 = varianza(vettore_elementi, media1);
	printf("La varianza è: %f.\n", varianza1);

	//Semaforo
	Signal_Sem(sem_id, SPAZIO_DISP, 5);

}
