#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../monitor/monitor.h"
#include "lib.h"

int main(){
	
	pid_t pid;
	int bufferID = shmget(IPC_PRIVATE, sizeof(Bs), IPC_CREAT|0664);
	Bs* buffer = (Bs*)shmat(bufferID, 0, 0);
	
	buffer->numLettori   = 0;
	buffer->numScrittori = 0;
	buffer->meteo.temperatura = 0;
	buffer->meteo.umidita     = 0;
	buffer->meteo.pioggia     = 0;
		
	init_monitor(&(buffer->m), NUM_COND);
	
	for(int i=0; i<NUM_PROC; i++){
		
		pid = fork();
		
		if(pid==0){
			if(i==0)
				for(int j=0; j<10; j++){
					servizio(buffer);
					sleep(2);
				}
			else{
				for(int j=0; j<10; j++){
					sleep(1);
					printf("Utente %d:\n", i);
					utente(buffer);
				}
			}
			_exit(0);
		}
	}
			
				
	for(int i=0; i<NUM_PROC; i++)
		wait(NULL);
	
	shmctl(bufferID, IPC_RMID, 0);
	remove_monitor(&(buffer->m));
	
	return 0;
}
