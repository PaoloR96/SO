/* Come 14_1_xxx starvation sia di scrittori che di lettori */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "lib.h"

int main(){
	
	//dichiarazione delle variabili
	key_t chiave_sem, chiave_shm;
	pid_t pid;
	Buffer *buffer;
	int i, k, id_sem, id_shm, status;

	printf("\n\n INIZIO \n\n");

	//assegno le chiavi 
	chiave_sem = IPC_PRIVATE;
	chiave_shm = IPC_PRIVATE;

	//creo semafori
	id_sem = semget(chiave_sem, 2, IPC_CREAT | 0664);
	semctl(id_sem, MUTEX_LETTORI, SETVAL, 1);
	semctl(id_sem, SYNCH, SETVAL, 1);

	//creo memoria condivisa
	id_shm = shmget(chiave_shm, sizeof(Buffer), IPC_CREAT | 0664);
	buffer = (Buffer*)shmat(id_shm, 0, 0);
	buffer->dato = 0;
	buffer->num_lettori = 0;
	buffer->cont_lettura = 0;
	buffer->cont_scrittura = 0;
	buffer->num_scrittori = 0;

	//stampo memoria condivisa
	printf(" dato <%d> num_lettori <%d> cont_lettura <%d> num_scrittori <%d> cont_scrittori <%d>\n",buffer->dato, buffer->num_lettori, buffer->cont_lettura, buffer->num_scrittori, buffer->cont_scrittura);
	printf("\n");	
	
	//creo processi figli
	for(i=0; i<NUM_PROC; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if(i%2 == 0){
				//SCRITTORE
				printf("Sono SCRITTORE pid <%d>\n",getpid());
				for(k=0; k<2; k++){
					Scrittore(id_sem, buffer);
					sleep(1);
				}
			}else{
				//LETTORE
				printf("Sono LETTORE pid <%d>\n",getpid());
				for(k=0; k<2; k++){
					Lettore(id_sem, buffer);
					sleep(1);
				}
			}
			exit(0);
		}
	}

	//attendo terminazione processi figli
	for(i=0; i<NUM_PROC; i++){
		pid = wait(&status);
		printf("Processo TERMIANTO <%d> con status <%d>\n", pid, status);
	}
	
	//stampo memoria condivisa
	printf("\n");
	printf(" dato <%d> num_lettori <%d> cont_lettura <%d> num_scrittori <%d> cont_scrittori <%d>\n",buffer->dato, buffer->num_lettori, buffer->cont_lettura, buffer->num_scrittori, buffer->cont_scrittura);

	//rimuovo memroia e semafori
	semctl(id_sem, 0, IPC_RMID);
	shmctl(id_shm, IPC_RMID, 0);	

	printf("\n\n FINE \n\n");

	return 0;
}
