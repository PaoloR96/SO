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
	
	key_t chiave_sem = 0;
	key_t chiave_shm = 0;
	int processo;
	Buffer *ptr_buff;
	int i;
	int resto;
	int status;
	int valore;
	int id_sem = 0;
	int id_shm = 0;
	
	//inizializza il semaforo e la memoria condivisa
	inizializza(&chiave_sem, &chiave_shm, &ptr_buff, &id_sem, &id_shm);
	
	printf("\nchiave semaforo <%d> \nchiave shm <%d> \nid semaforo <%d> \nid shm <%d>\n\n",chiave_sem, chiave_shm, id_sem, id_shm);
		
	stampaMemoria(ptr_buff);
	
	//genero 10 processi 5 produttori e 5 consumatori
	for(i=0; i<10; i++){
		processo = fork();
		resto = i%2;
		if(processo == 0){
			if(resto == 0){
				//produttore
				printf("sono processo figlio con pid <%d> numero <%d> PRODUTTORE\n",getpid(),i);
				produttore(ptr_buff,id_sem);
				_exit(0);
			}
			if(resto != 0){
				//consumatore
				printf("sono processo figlio con pid <%d> numero <%d> CONSUMATORE\n",getpid(),i);
				consumatore(ptr_buff,id_sem);
				_exit(0);
			}
		}
		if(processo == -1){
			printf("creazione fallita\n\n");
			rimozione(&id_sem,&id_shm);
			_exit(-1);
		}
	}
	
	//attendo che i figli siano terminati
	for(i=0; i<10; i++){
		processo = wait(&status);
	}
	
	
	
	
	//rimuovo i semafori e la memoria condivisa creata
	rimozione(&id_sem,&id_shm);
	
	printf("\n\n");
	return 0;
}