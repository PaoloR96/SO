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

int main(){

	srand(time(0));

	Thunderstruct *ACDC;

	int st, id_sem, id_struct, i;
	pid_t pid_ex, pid;
	key_t chiave_struct, chiave_sem;
	int num_scrittori = 1;
	int num_elaboratori = 1;
	int num_analizzatori = 2;
	int num_processi = num_scrittori + num_elaboratori + num_analizzatori;

	chiave_struct = ftok(PATH_SHM,CHAR_SHM);
	chiave_sem = ftok(PATH_SEM,CHAR_SEM);
	
	id_struct = shmget(chiave_struct, sizeof(Thunderstruct), IPC_CREAT|0664);
	id_sem = semget(chiave_sem, 3, IPC_CREAT|0664);

	ACDC = (Thunderstruct *) shmat(id_struct, 0, 0);

	ACDC->dim = 3 + rand()%3;

	semctl(id_sem, MUTEX, SETVAL, 1);
	//semctl(id_sem, MUTEXL, SETVAL, 1);
	semctl(id_sem, SYNCH, SETVAL, 0);
	
	for(i = 0; i < num_processi; i++){
		pid = fork();
	
		if(pid == 0){
			if(i == 0){
				printf("\n\nSono il processo che aggiorna numero %d con pid %d\n", i+1, getpid()); 
				execl("./generatore", "./generatore", NULL);
				_exit(1);
			}	
			
			if(i == 1){
				printf("\n\nSono il processo elaboratore numero %d con pid %d\n", i, getpid());
				execl("./elaboratore", "./elaboratore", NULL);	
				_exit(1);		
			}

			else {
				printf("\n\nSono il processo analizzatore numero %d con pid %d\n", i-1, getpid());
				execl("./analizzatore", "./analizzatore", NULL);	
				_exit(1);		
			}	
		}
	}

	printf("Sono il padre con pid %d\n", getpid());	
	sleep(15);

	for(i = 0; i < num_processi; i++){
		if(pid == -1){
			perror("Si e' verificato un errore!\n");
			_exit(1);		
		} else if (pid > 0){	
			ACDC->condizione = 1;
			pid_ex = wait(&st);
			printf("Il processo con pid %d ha terminato la sua esecuzione con stato %d\n", pid_ex, st>>8);
		}
	}
	
	shmctl(id_struct, IPC_RMID, 0);
	semctl(id_sem, 0, IPC_RMID);
	
	return 0;
}

