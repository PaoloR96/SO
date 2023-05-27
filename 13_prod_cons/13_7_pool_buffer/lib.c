#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "lib.h"

//gestione semafori
void signal(int id_sem, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = 1;
	semop(id_sem, &sem, 1);	
}

void wait(int id_sem, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = -1;
	semop(id_sem, &sem, 1);	
}

//gestione produttori e consumatori
void produttore(int id_sem, Buffer *buffer, BufferStato *bufferStato){
	
	//variabile locale	
	int i = 0;
	
	//attendo SPAZIO_DISP ed entro in MUTEX_PROD su bufferStato
	wait(id_sem, SPAZIO_DISP);
	wait(id_sem, MUTEX_PROD);
	
	//controllo finchè non trovo una cella del bufferStato contente VUOTO
	while((i<DIM)&&(bufferStato->vettoreStato[i] != VUOTO)){
		i++;
	}

	//setto la cella trovata a pieno in modo da essere l'unico a utilizzarla
	bufferStato->vettoreStato[i] = PIENO;

	//libero il MUTEX_PROD, che potranno accedervi ma non useranno la cella che sto usando io
	signal(id_sem, MUTEX_PROD);	
	
	//produco un valore nella cella trovata	
	buffer->vettore[i] = rand()%123;
	printf(" produco dato <%d>\n",buffer->vettore[i]);

	//segnalo ai consumatori che ho prodotto MSG_DISP	
	signal(id_sem, MSG_DISP);
}

void consumatore(int id_sem, Buffer *buffer, BufferStato *bufferStato ){
	//variabile locale	
	int i = 0;
	
	//attendo MSG_DISP ed entro in MUTEX_PROD su bufferStato
	wait(id_sem, MSG_DISP);
	wait(id_sem, MUTEX_CONS);
	
	//controllo finchè non trovo una cella del bufferStato contente VUOTO
	while((i<DIM)&&(bufferStato->vettoreStato[i] != PIENO)){
		i++;
	}

	//setto la cella trovata a pieno in modo da essere l'unico a utilizzarla
	bufferStato->vettoreStato[i] = IN_USO;

	//libero il MUTEX_PROD, che potranno accedervi ma non useranno la cella che sto usando io
	signal(id_sem, MUTEX_CONS);	
	
	//produco un valore nella cella trovata	
	printf("  Leggo Messaggio <%d>\n",buffer->vettore[i]);
	buffer->vettore[i] = 0;
	bufferStato->vettoreStato[i] = VUOTO;

	//segnalo ai consumatori che ho prodotto MSG_DISP	
	signal(id_sem,SPAZIO_DISP);
}
