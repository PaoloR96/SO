/*	Lettori Scrittori 
 *	Abbiamo più processi scrittori e più processi lettori, dove i processi scrittori possono
 *	leggere contemporamentamente il dato scritto    dallo scrittore, mentre gli scrittori possono
 *	accedere uno per volta per scrivere e se la risorsa è occupata da un lettore non vi possono accedere
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "lib.h"

int main(){

	//dichiarazione variabili
	key_t chiave_sem;
	key_t chiave_shm;
	pid_t pid;
	Buffer *buffer;
	int id_shm;
	int id_sem;
	int status;
	int i;
	int k;

	printf("\n\n INIZIO \n\n");

	//assegnazione chiavi
	chiave_sem = IPC_PRIVATE;
	chiave_shm = IPC_PRIVATE;
	
	//semafori
	id_sem = semget(chiave_sem, 3, IPC_CREAT | 0664);
	semctl(id_sem, MUTEX_LETTORI, SETVAL, 1);
	semctl(id_sem, SYNCH, SETVAL, 1);

	//memoria condivisa
	id_shm = shmget(chiave_shm, sizeof(Buffer), IPC_CREAT | 0664);
	buffer = (Buffer*)shmat(id_shm, 0, 0);

	//inizializzo memoria condivisa
	buffer->dato = 0;
	buffer->num_lettori = 0;	
	buffer->cont_lettura = 0;

	//stampo il buffer	
	printf("\nstampo buffer \n");
	printf(" dato: <%d> num lettori: <%d>\n", buffer->dato, buffer->num_lettori);

	printf("\n");

	//creazione dei figli
	for(i=0; i<NUM_PROC; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if(i%2 == 0){
				//Scrittore
				printf("sono processo SCRITTORE <%d>\n",getpid());
				scrittore(id_sem, buffer);	
			}else{
				//Lettore
				printf("sono processo LETTORE <%d>\n",getpid());				
				//for(k=0; k<2; k++)  //per leggere più volte 
				lettore(id_sem, buffer);
			}
			_exit(0);
		}
	}

	//attendo terminazione dei figli
	for(i=0; i<NUM_PROC; i++){
		pid = wait(&status);
		printf("processo <%d> TERMINATO con stato <%d>\n", pid, status);
	}

	//rimozione memoria condivisa e semafori
	shmctl(id_shm, IPC_RMID, 0);
	semctl(id_sem, 0, IPC_RMID);

	//stampo il buffer	
	printf("\nstampo buffer \n");
	printf(" dato: <%d> num lettori: <%d> numero letture effettuate [%d]\n", buffer->dato, buffer->num_lettori, buffer->cont_lettura);

	printf("\n\n FINE \n\n");

	return 0;
}
