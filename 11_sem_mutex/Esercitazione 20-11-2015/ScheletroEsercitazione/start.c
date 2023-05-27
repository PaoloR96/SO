#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header.h"

#define N_PROC 2


int main(){
	pid_t pid_ex,pid;
	int st,i;
	//Istanziamo 
	for(i = 0; i < N_PROC;i++){
		pid = fork();
		if(pid == 0){
			if(i == 0){
			//Instanzia consumatore
			}else{
			//Instanzia produttore
			}
			_exit(0);			
		}
		sleep(1);

	}
	
	for(i = 0; i < N_PROC ; i++){
		pid_ex = wait(&st);
		printf("[MASTER] - Il processo %d ha terminato l'esecuzione\n",pid_ex);			
	}
	return 0;
}
