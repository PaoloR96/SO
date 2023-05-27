/* 	1 consumatore e più produttori, i produttori entrano in mutua esclusione
 *	sulla risorsa, per produrre fino a riempirla, il consumatore per consumare attende 
 *	che sia piena e poi consuma tutto il buffer rimepito, avverte i cosumatori che torneranno 
 *	a produrre 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main(){

	//dichiarazione delle variabili
	key_t chiave_shm; 
	key_t chiave_sem;
	pid_t pid;
	Buffer *buffer;
	int id_sem;
	int id_shm;
	int status;
	int i;
	int k;

	printf("\n\n ____INIZIO____ \n\n");

	//assegnazione chiavi
	chiave_sem = ftok(".",'a');
	chiave_shm = ftok(".",'b');

	//creazione memoria condivisa
	id_shm = shmget(chiave_shm, sizeof(Buffer), IPC_CREAT | 0664);
	buffer = (Buffer*)shmat(id_shm, 0, 0);

	//inizializzo variabili
	buffer->testa = 0;
	buffer->coda = 0;

	//creazione dei semafori	
	id_sem = semget(chiave_sem, 3, IPC_CREAT | 0664);
	semctl(id_sem, MUTEX, SETVAL, 1);
	semctl(id_sem, MSG_DISP, SETVAL, 0);
	semctl(id_sem, SPAZIO_DISP, SETVAL, DIM);

	//creazione dei processi
	for(i=0; i<NUM_PROC; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if(i == 0){
				//consumatore
				printf("Sono CONSUMATORE <%d>\n",getpid());
				execl("./consumatore", "consumatore", NULL);
			}else{
				//produttori
				printf("Sono PRODUTTORE <%d>\n",getpid());
				execl("./produttore", "produttore", NULL);
			}
			exit(0);
		}
	}

	//attendo terminazione dei processi
	for(i=0; i<NUM_PROC; i++){
		pid = wait(&status);
		printf("-Processo TERMINATO <%d> con status <%d>\n", pid, status);
	}	

	//rimozione memoria e semafori
	semctl(id_sem, 0, IPC_RMID);
	shmctl(id_shm, IPC_RMID, 0);

	printf("\n ____FINE____ \n\n");

	return 0;
}
