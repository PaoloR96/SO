/*	Il processo peoduttore può produrre solo se il bufffer e vuoto, metre il processo consumatore
 *	può consumare solo quando il buffer e pieno e qunado consuma lo svuota completamente.
 *	N.B. il numero di consumazione deve sempre essere uguale o minore al numero di prduzioni vedi file.h 
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

	//dichiarazione delle variabili 
	key_t chiave_sem;
	key_t chiave_shm;
	pid_t pid;
	Buffer *buffer;
	int status;
	int id_shm;
	int id_sem;
	int i;
	int k;

	printf("\n\n INIZIO \n\n");
	
	//asseganzione delle chiavi
	chiave_sem = IPC_PRIVATE;
	chiave_shm = IPC_PRIVATE;
	
	//creazione memoria condivisa
	id_shm = shmget(chiave_shm, sizeof(Buffer), IPC_CREAT | 0664);
	buffer = (Buffer*) shmat(id_shm,0,0);	
	
	//inizializzazione buffer
	for(i=0; i<DIM; i++){
		buffer->vettore[i] = 0;
	}

	//creazione semafori
	id_sem = semget(chiave_sem, 3, IPC_CREAT | 0664);
	semctl(id_sem, MUTEX, SETVAL, 1);
	semctl(id_sem, PRODUCI, SETVAL, 1);
	semctl(id_sem, CONSUMA, SETVAL, 0);

	//stampo il buffer
	printf("stampo il buffer\n");
	for(i=0; i<DIM; i++){
		printf("<%d>\n",buffer->vettore[i]);
	}
	
	printf("\n");

	//creazione processi figli
	for(i=0; i<NUM_PROC; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if( i == 0){
				//processo produttore
				printf("Sono processo PRODUTTORE pid <%d>\n",getpid());
				for(k=0; k<NUM_PROD; k++){
					sleep(1);
					produttore(id_sem, buffer);
				}
			}else{
				//processo consumatore
				printf("Sono processo CONSUMATORE pid <%d>\n",getpid());
				for(k=0; k<NUM_CONS; k++){
					sleep(3);
					consumatore(id_sem, buffer);
				}
			}		

			_exit(0);		
		}
	}
	
	//attendo terminazione dei processi figli
	for(i=0; i<NUM_PROC; i++){
		pid = wait(&status);
		printf("processo <%d> TERMINATO stato <%d>\n",pid,status);
	}

	//stampo il buffer
	printf("\nstampo il buffer\n");
	for(i=0; i<DIM; i++){
		printf("<%d>\n",buffer->vettore[i]);
	}

	//rimuovo struttura smaforica e memoria
	semctl(id_sem, 0, IPC_RMID);
	shmctl(id_shm, IPC_RMID, 0);

	printf("\n\n FINE \n\n");	
	
	return 0;
}
