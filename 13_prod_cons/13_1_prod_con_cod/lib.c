#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "lib.h"


//gestione del semaforo
void WaitSem(int id_sem, int num_sem){
	struct sembuf sem_buf;
	sem_buf.sem_num = num_sem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = -1;
	//semaforo rosso
	semop(id_sem, &sem_buf, 1);	
}

void SignalSem (int id_sem, int num_sem){
	struct sembuf sem_buf;
	sem_buf.sem_num = num_sem;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = 1;
	//semaforo verde
	semop(id_sem, &sem_buf, 1);
}


//produce un dato da inserire in coda nel nostro Buffer
void Produttore(int *coda , msg *buffer, int id_sem){
	
	//entriamo in mutua esclusione tra i processi produttori wait MUTEX_PROD
	//attendo e decrementiamo lo spazio con la wait SPAZIO_DISP
	WaitSem(id_sem, SPAZIO_DISP);	
	WaitSem(id_sem, MUTEX_PROD);
	
	//produciamo un valore random e lo inseriamo 
	buffer[(*coda)] = rand()%234;
	printf("Produco valore [%d] prodotto da <%d>\n",buffer[(*coda)],getpid());

	//incrementiamo la coda (in questo modo se arriva all'ultimo elemento ritorna in testa
	(*coda) =++ (*coda)%DIMENSIONE;
	
	//avviso ai processi produttori che ho finito signal MUTEX_PROD
	//avviso ai consumatori che c'è un nuovo messaggio signal MSG_DISP
	SignalSem(id_sem, MUTEX_PROD);
	SignalSem(id_sem, MSG_DISP);

}


//legge un dato dalla testa dal nostro Buffer
void Consumatore(int *testa,msg *buffer, int id_sem){

	//entriamo in mutua esclusione tra i processi consumatori wait MUTEX_CONS
	//attendo e decrementiamo i messaggi disponibili con la wait MSG_DISP
	WaitSem(id_sem, MSG_DISP);
	WaitSem(id_sem, MUTEX_CONS);
	
	//leggo elemento in testa e incremento la testa
	printf("Messanngio letto : [%d] da <%d>\n",buffer[(*testa)],getpid());
	(*testa) =++(*testa)%DIMENSIONE;
	
	//avviso ai processi consumatori che ho finito signal MUTEX_CONS
	//avviso ai produttori che c'è un nuovo spazio disponibile signal SPAZIO_DISP
	SignalSem(id_sem, MUTEX_CONS);
	SignalSem(id_sem, SPAZIO_DISP);
}
