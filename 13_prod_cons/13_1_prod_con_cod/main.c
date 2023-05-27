/* 	Produttori Consumatori, più processi produttori diversi producono 
 *	dati inseriti in un Buffer nella sua coda, mentre più 
 *	processi diversi consumatori leggono i dati presenti nel buffer 
 *	
 *	NB. Il programma funzione solo se il numero di produzioni resta minore
 *	uguale alla dimensione del buffer e il numero di consumi resta minore
 *	o uguale al numero di consumazioni (VEDERE I #define nel file lib.h).
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include "lib.h"

int main(){
	
	//dichiarazione delle variabili
	int num_processi;
	int status;
	int i;
	int k;
	int id_sem;
	int id_shm;
	int *testa;
	int *coda;
	msg *buffer;
	key_t key_sem;
	key_t key_shm;
	pid_t pid;
	
	num_processi = 4;

	//assegno le chiavi
	key_sem = IPC_PRIVATE;
	key_shm = IPC_PRIVATE;

	//memoria condivisa
	id_shm = shmget(key_shm, (DIMENSIONE*(sizeof(msg)+2*(sizeof(int)))), IPC_CREAT | IPC_EXCL | 0664);
	printf("\n\nid memoria : <%d>\n",id_shm);
	
	//faccio un controllo sulla memoria condivisa se fallisce
	if(id_shm == -1){
		//provo a richiamarla 
		shmget(key_shm, (DIMENSIONE*(sizeof(msg)+2*(sizeof(int)))), 0664);
		//se fallisce nuovamente mando messaggio di errore
		if(id_shm == -1){
			perror("errore memmoria no creata");
		}	
	}
	//inizializzo la memoria
	//riserva a testa una porzione di memoria di intero
	testa = (int*)shmat(id_shm,NULL,0);
	//riservo a coda la porzione di intero successivo a testa (testa è un puntataore percio)
	coda = testa+1;
	//riservo memoria al resto del buffer a partire da coda fino alla fine riserva memoria di grandezza msg
	buffer = (msg*)(coda+1);
	//inizializzo il valore dei puntatori
	(*testa) = 0;
	(*coda) = 0;

	//semafori
	id_sem = semget(key_sem,4,IPC_CREAT | 0664);
	printf("id semaforo : <%d>\n\n",id_sem);
	//garantisco mutua esclusione tra i produtori/consumatori dicendo che ne puo entrare 1 solo per volta (SETVAL,1)
	semctl(id_sem,MUTEX_PROD,SETVAL,1);
	semctl(id_sem,MUTEX_CONS,SETVAL,1);
	//inizialmente abbiamo 0 messaggi disponibili per i consumaori e la dimensione massima del buffer essendo vuoto
	semctl(id_sem,SPAZIO_DISP,SETVAL,DIMENSIONE);
	semctl(id_sem,MSG_DISP,SETVAL,0);
	
	//creazione dei processi figlio (produttore e consumatore)
	for(i=0; i<num_processi; i++){
		pid = fork();
		//serve a far generare alla funzione rand() nei processi figli (lib.c) numeri diversi
		srand(time(NULL)^getpid());
		if(pid == 0){
			if(i%2==0){
				//produttori
				printf("Sono PRODUTTORE con pid <%d>\n",getpid());	
				for(k=0; k<NUM_PRODUZIONI; k++){
					sleep(1);
					Produttore(coda, buffer, id_sem);	
				}
			}else{
				//consumatori
				printf("Sono CONSUMATORE con pid <%d>\n",getpid());
				for(k=0; k<NUM_CONSUMAZIONI; k++){
					sleep(4);
					Consumatore(testa, buffer, id_sem);	
				}
			}
			_exit(0);		
		}	
	}

	//attendo la terminazione dei processi
	for(i=0; i<num_processi; i++){
		pid = wait(&status);
		printf("processo <%d> TERMINATO con stato <%d>\n",pid,status);	
	}
	
	//rimozione semafori e memoria condivisa
	semctl(id_sem,0,IPC_RMID);
	shmctl(id_shm,IPC_RMID,0);
	
	printf("\nFine programma PADRE \n\n");	

	return 0;
}
