#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "header.h"

int main(){
	
	
	int id_sem, id_struct;
	key_t chiave_struct, chiave_sem;
	Thunderstruct *ACDC;
	
	chiave_struct = ftok(PATH_SHM,CHAR_SHM);
	chiave_sem = ftok(PATH_SEM,CHAR_SEM);

	id_struct = shmget(chiave_struct, 0, IPC_CREAT|0664);
	id_sem = semget(chiave_sem, 0, IPC_CREAT|0664);

	ACDC = (Thunderstruct*)shmat(id_struct, 0, 0);

	ACDC->condizione = 0;
	
	while(ACDC->condizione != 1){
		raddoppio(ACDC, id_sem);
		sleep(1);
	}	

	return 1;
}
