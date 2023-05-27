#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "./Monitor/monitor.h"
#include "lib.h"

int main(){
	
	//dichiarazione delle variabili
	pid_t pid;
	key_t chiave_shm;
	MonitorLettScritt *monitor_ls;
	int id_shm;
	int status;
	int i;
	int k;

	sleep(1);

	printf("\n\n __INIZIO__ \n\n");	

	//assegno chiavi
	chiave_shm = IPC_PRIVATE;

	//memoria condivisa
	id_shm = shmget(chiave_shm, sizeof(MonitorLettScritt), IPC_CREAT | 0664 );
	monitor_ls = (MonitorLettScritt*)shmat(id_shm, 0, 0);
	
	//inizializzo memoria
	monitor_ls->dato = 0;	
	monitor_ls->num_lettori = 0;
	monitor_ls->occupato = 0;

	//inizializzo il monitor do l'indirizzo del monitor presente nella struct MonitorLettScritt
	init_monitor(&(monitor_ls->monitor), NUM_CONDITIONS);
	
	//creazione processi 
	for(i=0; i<NUM_PROC; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if( i%2 == 0 ){
				//Scrittore
				printf("Sono SCRITTORE <%d>\n", getpid());
				Scrittore(monitor_ls);
			}else{
				//Lettore
				printf("Sono LETTORE <%d>\n", getpid());
				Lettore(monitor_ls);
			}
		exit(0);
		}
	}

	//attendo terminazione dei processi
	for(i=0; i<NUM_PROC; i++){
		pid = wait(&status);
		printf("--processo <%d> TERMINATO status <%d> \n", pid, status);
	}

	//rimozione monitor e memeoria condivisa
	shmctl(id_shm, IPC_RMID, 0);
	remove_monitor( &(monitor_ls->monitor) );
	
	printf("\n\n __FINE__ \n\n");	

	return 0;
}
