/*	Stesso meccanismo degli altri esercizi 13_x_... solo che abbiamo 
 *	N produtore e 1 consumatori, i consumatori si alternano all'entrata nella
 *	memoria condivisa per consumare il dato prodotto
 *	notare che si deve mantenere il rapporto tra consumazioni e produzioni nel file.h altrimenti 
 *	non funzione perche fa piu consumazioni di quanti valori prodotti e quindi resta 
 *	in un attesa permanente starvation
 */

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

int main(){
	
	//dichiarazione variabili 
	Buffer *buffer;
	key_t chiave_sem;
	key_t chiave_shm;
	pid_t pid;
	int id_sem;
	int id_shm;
	int i;
	int k;
	int status;

	//assengo le chiavi
	chiave_sem = IPC_PRIVATE;
	chiave_shm = IPC_PRIVATE;

	//creo memoria condivisa ho omesso i controlli di creazione	
	id_shm = shmget(chiave_shm, sizeof(Buffer), IPC_CREAT | 0664);
	buffer = (Buffer*)shmat(id_shm,0,0);
	buffer->testa = 0;
	buffer->coda = 0;
	
	//setto il buffer con tutti i valore a 0
	for(i=0; i<DIM; i++){
		buffer->vettore[i] = 0;	
	}

	//creazione dei semafori
	id_sem = semget(chiave_sem, 4, IPC_CREAT | 0664);
	semctl(id_sem, MUTEX_PROD, SETVAL, 1);
	semctl(id_sem, SPAZIO_DISP, SETVAL, DIM);
	semctl(id_sem, MSG_DISP, SETVAL, 0);
	
	printf("\n\n INIZIO \n\n");

	//stampo il buffer	
	printf("stampo buffer\n");
	for(i=0; i<DIM; i++){
		printf("	<%d>\n",buffer->vettore[i]);	
	}
	printf("\n");

	for(i=0; i<NUM_PROCESSI; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if(i == 0){
				//un solo processo consumatore che fa piu consumazioni
				printf("Sono CONSUMATORE pid <%d>\n",getpid());
				for(k=0; k<NUM_CONS; k++){
					consumatore(id_sem, buffer);	
				}
			}else{
				//più processi porduttore
				printf("Sono PRODUTTORE pid <%d>\n",getpid());
				for(k=0; k<NUM_PROD; k++){
					sleep(2);
					produttore(id_sem, buffer);
					
				}
			}
			_exit(0);
		}
	}

	//attendo la temrinazione dei processi
	for(i=0; i<NUM_PROCESSI; i++){
		pid = wait(&status);
		printf("processo <%d> TERMINATO staus <%d> \n", pid, status);
	}	
	
	//stampo il buffer
	printf("\nstampo buffer\n");
	for(i=0; i<DIM; i++){
		printf("	<%d>\n",buffer->vettore[i]);	
	}
	printf("\n");
		
	//rimuovo strutture semaforiche e memoria condivisa
	semctl(id_sem, 0, IPC_RMID);
	shmctl(id_shm, IPC_RMID, 0);

	printf("\n\n FINE \n\n");	
	
	return 0;
}
