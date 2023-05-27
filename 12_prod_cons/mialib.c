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


//vado ad inizializzare i semafori e la memoria condivisa
void inizializza(key_t *chiave_sem ,key_t *chiave_shm ,Buffer **ptr_buff, int *id_sem, int *id_shm){
	
	//assegno le chiavi
	*chiave_sem = ftok(".",'l');
	*chiave_shm = ftok(".",'d');
	
	//inizializzo i semafori
	*id_sem = semget(*chiave_sem, 4, IPC_CREAT | IPC_EXCL | 0664);
	if(*id_sem == -1){
		//va a richiamare quelo gia esistente
		*id_sem = semget(*chiave_sem,4,0664);
		if(*id_sem == -1){
			perror("errore nella creazione del semaforo");
			_exit(-1);
		}
	}else{
		//inizzializzo semafori
		semctl(*id_sem, MUTEX_PROD, SETVAL, 1);
		semctl(*id_sem, MUTEX_CONS, SETVAL, 1);
		semctl(*id_sem, SPAZIO_DISP, SETVAL, 2);
		semctl(*id_sem, MSG_DISP, SETVAL, 0);
	}
	
	//inizializzo il sgmento di memoria
	*id_shm = shmget(*chiave_shm, sizeof(Buffer), IPC_CREAT | IPC_EXCL | 0664);
	if(*id_shm >=0 ){
		//attach
		(*ptr_buff) = (Buffer*)shmat(*id_shm,NULL,0);
	}
	else{
		//già esistente la richiamo
		*id_shm = semget(*chiave_sem,sizeof(Buffer),0664);
		if(*id_shm == -1){
			perror("errore nella creazione della memoria");
			_exit(-1);
		}
		//attach
		(*ptr_buff) = (Buffer*)shmat(*id_shm,NULL,0);
	}
	//inizializzazione
	(*ptr_buff)-> msg1 = 0;
	(*ptr_buff)-> msg2 = 0;
	(*ptr_buff)-> stato_msg1 = VUOTO;
	(*ptr_buff)-> stato_msg2 = VUOTO;
}




//rimuove le strutture dati semafro e memoria condivisa
void rimozione(int *id_sem,int*id_shm){
	
	semctl(*id_sem, 4, IPC_RMID);
	shmctl(*id_shm, IPC_RMID,0);
}






//fa la wait sul semaforo 
void wait(int id_semaforo, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = -1;
	semop(num_sem, &sem,1);
}





//fa la signal sul semaforo
void signal(int id_semaforo, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = 1;
	semop(num_sem, &sem,1);
}






//produttore deve produrre un numero intero da inserire in memoria condivisa
void produttore(Buffer *ptr_shm, int id_sem ){
	
	wait(id_sem, SPAZIO_DISP);
	wait(id_sem, MUTEX_PROD);
	
	if(ptr_shm->stato_msg1 == VUOTO){
		ptr_shm->msg1 = rand()%234;
		ptr_shm->stato_msg1 = PIENO;
	}else{
		ptr_shm->msg2 = rand()%153;
		ptr_shm->stato_msg2 = PIENO;
	}
	stampaMemoria(ptr_shm);
	
	signal(id_sem, MUTEX_PROD);
	signal(id_sem, MSG_DISP);
	
}


//consumatore deve consumare un numero intero in memoria condivisa
void consumatore(Buffer *ptr_shm, int id_sem ){
	
	wait(id_sem, MSG_DISP);
	wait(id_sem, MUTEX_CONS);
	
	if(ptr_shm->stato_msg1 == PIENO){
		ptr_shm->msg1 = 0;
		ptr_shm->stato_msg1 = VUOTO;
	}else{
		ptr_shm->msg2 = 0;
		ptr_shm->stato_msg2 = VUOTO;
	}
	stampaMemoria(ptr_shm);
	
	signal(id_sem, MUTEX_CONS);
	signal(id_sem, SPAZIO_DISP);
	
}


//stampa il contenuto della memoria condivisa
void stampaMemoria(Buffer* ptr_shm){
	
	char pieno[] = "PIENO";
	char vuoto[] = "VUOTO";
	
	printf("msg1       <%d>     processo <%d>\n",ptr_shm->msg1, getpid());
	if(ptr_shm->stato_msg1 == VUOTO)
		printf("stato_msg1 <%s> processo <%d>\n",vuoto, getpid());
	else
		printf("stato_msg1 <%s> processo <%d>\n",pieno, getpid());
	
	printf("msg2       <%d>     processo <%d>\n",ptr_shm->msg2, getpid());
	if(ptr_shm->stato_msg2 == VUOTO)
		printf("stato_msg1 <%s> processo <%d>\n",vuoto, getpid());
	else
		printf("stato_msg1 <%s> processo <%d>\n",pieno, getpid());
	printf("\n");
}

