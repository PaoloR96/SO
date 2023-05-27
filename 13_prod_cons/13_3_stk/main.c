/*Programma uguale ai precedenti 13_x_ ... ma con stack anziche coda circolare*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "lib.h"

int main(){

	//variabili
	int id_sem;
	int id_shm;
	int status;
	int i;
	int k;
	Pila *buffer;
	key_t chiave_sem;
	key_t chiave_shm;
	pid_t pid;
	
	//assegno le chiavi
	chiave_sem = IPC_PRIVATE;
	chiave_shm = IPC_PRIVATE;
	
	//assegno memoria condivisa 
	id_shm = shmget(chiave_shm, sizeof(Pila), IPC_CREAT | IPC_EXCL | 0664);
	printf("\n\nid memoria <%d>\n\n", id_shm);

	//controllo creazione memoria e inizializzo
	if(id_shm == -1){
		shmget(chiave_shm, sizeof(Pila), 0664);
		if(id_shm == -1){
			perror("errore creazione della memoria");	
		}
	}
	//inizializzazione della memoria
	buffer = (Pila*)shmat(id_shm,0,0);
	buffer->testa = 0;

	//inizializzo il buffer con tutti 0
	printf("inizializzo la mia Pila \n");
	for(i = 0; i<DIMENSIONE ; i++){
		buffer->vettore[i] = 0;	
	}

	//stampo la Pila
	for(i = 0; i<DIMENSIONE ; i++){
		printf("valore <%d> pos [%d]\n",buffer->vettore[i],i);	
	}
	
	//assegno i semafori
	id_sem = semget(chiave_sem, 4, IPC_CREAT | IPC_EXCL | 0664);
	printf("\nid semaforo <%d>\n\n", id_sem);	
	semctl(id_sem, MUTEX_CONS, SETVAL, 1);
	semctl(id_sem, MUTEX_PROD, SETVAL, 1);
	semctl(id_sem, MSG_DISP, SETVAL, 0);
	semctl(id_sem, SPAZIO_DISP, SETVAL, DIMENSIONE);
	
	//generazione dei processi produttore e consumatore
	for(i=0; i<NUM_PROCESSI; i++){
		pid = fork();
		srand(time(NULL)^getpid());
		if(pid == 0){
			if(i%2 == 0){
				//processo produttore
				printf("Sono processo PRODUTTORE pid <%d>\n",getpid());
				for(k=0; k<NUM_PROD; k++){
					//produzione dati
					//sleep(1);
					produttore(id_sem,buffer);
				}				
			}else{
				//processo consumatore
				printf("Sono processo CONSUMATORE pid <%d>\n",getpid());
				for(k=0; k<NUM_CONS; k++){
					//consumo dati
					//sleep(2);
					consumatore(id_sem,buffer);
				}
			}
			_exit(0);			
		}
	}

	//attendo terminazione dei processi
	for(i=0; i<NUM_PROCESSI; i++){
		pid = wait(&status);
		printf("Processo pid <%d> TERMINATO con status <%d>\n",pid,status);
	}	

	printf("\n");	

	//stampo la Pila
	printf("stampo la pila risultato finale\n");
	for(i = 0; i<DIMENSIONE ; i++){
		printf("valore <%d> pos [%d]\n",buffer->vettore[i],i);	
	}
	
	printf("\n");		

	//rimozione memoria e semafori
	printf("rimuovo semafori e memoria condivisa");
	shmctl(id_shm, IPC_RMID, 0);
	semctl(id_sem,0,IPC_RMID);

	//messaggio di terminazione del prgramma padre
	printf("\n\nFINE PROGRAMMA\n\n");	
	
	return 0;
}
