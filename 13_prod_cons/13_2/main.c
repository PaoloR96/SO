/*Produttore consumatore identico al 31_1_prod_con_coda ma la struttura coda e fatta in modo diverso vedere file.h (lib.h)*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib.h"

int main(){
	
	//dichiarazione delle variabili
	Buffer *buffer;
	key_t chiave_shm;
	key_t chiave_sem;
	pid_t pid;
	int id_sem;
	int id_shm;
	int status;
	int i;
	int k;
	
	//inizializzo le chiavi
	chiave_shm = IPC_PRIVATE;
	chiave_sem = IPC_PRIVATE;
	
	//inizializzo memoria condivisa
	id_shm = shmget(chiave_shm,sizeof(Buffer),IPC_CREAT | 0664);
	buffer = (Buffer*)shmat(id_shm,0,0);
	buffer-> testa = 0;
	buffer -> coda = 0;
		
	//inizializzo tutti gli elemnti del Buffer a 0
	for(i=0; i<DIMENSIONE; i++){
		buffer->vettore[i] = 0;
	}

	printf("\n\n");	

	//stampa del Buffer
	for(i=0; i<DIMENSIONE; i++){
		printf("valore <%d> pos [%d]\n",buffer->vettore[i],i);
	}

	//inizializzo semafori
	id_sem = semget(chiave_sem,4,IPC_CREAT | 0664);
	semctl(id_sem, MUTEX_PROD, SETVAL,1);
	semctl(id_sem, MUTEX_CONS, SETVAL,1);
	semctl(id_sem, SPAZIO_DISP, SETVAL,DIMENSIONE);
	semctl(id_sem, MSG_DISP, SETVAL,0);
	
	//genero processi 
	for(i=0; i<NUM_PROCESSI; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if(i%2 == 0){
				//PRODUTTORE
				printf("Processo PRODUTTORE <%d>\n",getpid());
				for(k=0; k<NUM_PRODUZIONI; k++){
					sleep(1);
					Produttore(id_sem, buffer);
				}
			}else{
				//CONSUMATORE
				printf("Processo CONSUMATORE <%d>\n",getpid());
				for(k=0; k<NUM_CONSUMAZIONI; k++){
					sleep(2);
					Consumatore(id_sem, buffer);
				}	
			}
			_exit(0);	
		}	
	}
	
	//attendo terminazione dei figli
	for(i=0; i<NUM_PROCESSI; i++){
		pid = wait(&status);
		printf("Processo <%d> TERMINATO con status [%d]\n",pid,status);
	}
	
	//stampa del Buffer
	for(i=0; i<DIMENSIONE; i++){
		printf("valore <%d> pos [%d]\n",buffer->vettore[i],i);
	}

	printf("\n\nTERMINO PROCESSO PADRE\n\n");
		
	//rimozione memoria condivisa e semafori
	semctl(id_sem, 0, IPC_RMID);	
	shmctl(id_shm,IPC_RMID,0);	
	
	return 0;
}
