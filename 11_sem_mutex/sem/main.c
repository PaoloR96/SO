/*genero una memoria condivisa e dei processi che a turno entreranno nella memoria codivisa e incrementaranno di uno il valore presente all'interno*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "mialib.h"

int main(){
	
	int id_semaforo;					//semaforo
	int id_shm;						//memori share (shm)
	int k;							//contatore
	int status;						//stato dei processi 
	msg *ptr_shm;						//puntatore al ipo di dato struct creato
	pid_t pid; 						//dichiaro un pid
	
	key_t chiave_sem = IPC_PRIVATE;		//assegno chiavi private 
	key_t chiave_shm = IPC_PRIVATE;		//assegno chiavi private 
	
	printf("\n\n");
	
	//faccio richiesta di una memoria condivisa che ha grandezza di una variable di tipo msg
	id_shm = shmget(chiave_shm,sizeof(msg),IPC_CREAT | 0664);
	//inizializzo la memoria condivisa e faccio attach al ptr_shm (struct msg)
	ptr_shm = (msg*)(shmat(id_shm,0,0));
	ptr_shm -> valore = 0;
	ptr_shm -> modificatoDa = 0;
	
	//faccio richiesta del semaforo
	id_semaforo = semget(chiave_sem, 1, IPC_CREAT | 0664);
	//inizializzo il semaforo vedere MUTEX = 0 mialib.h
	semctl(id_semaforo,MUTEX,SETVAL,1);
	
	//genero 5 figli
	for(k=0;k<5;k++){
		pid = fork();
		if(pid == 0 ){
			//processo figlio
			printf("\nSono il figlio <%d> con pid <%d>\n",k,getpid());
			waitSem(id_semaforo,MUTEX);
			usoRisorsa(ptr_shm);
			signalSem(id_semaforo,MUTEX);
			exit(0);
		}
	}
	
	//aspetto la terminazione dei figli
	for( k=0; k<5; k++ ){
		pid = wait(&status);
		if(pid == -1)
			perror("figlio temrinato con errore");
		else
			printf("figlio <%d> terminato con stato : %d\n", pid, status);
	}
	
	printf("\nValore finale della risorsa : %ld modificato da %d\n\n",ptr_shm->valore,ptr_shm->modificatoDa);
	
	//rimuovo memoria condivisa e mutex
	shmctl(id_shm,IPC_RMID,0);
	semctl(id_semaforo,0,IPC_RMID);
	
	return 0;
}



