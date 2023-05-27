#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main(){
	
	//Area codice matrice

	int n,m;
	
	printf("Inserisci il numero di righe: ");
	scanf("%d", &n);
	printf("\nInserisci il numero di colonne: ");
	scanf("%d", &m);
	printf("\n");
	
	int mat[dmax2][dmax2];
	scrivi_matrice(n, m, mat);	
	leggi_matrice(n, m, mat);
	
	//Chiavi
	
	key_t chiave_buffer = IPC_PRIVATE; 
	key_t chiave_stato = IPC_PRIVATE;
	key_t chiave_sem = IPC_PRIVATE;
	key_t chiave_zero = IPC_PRIVATE;

	pid_t pid;
	int* stato;
	int id_buffer, num_processi, id_stato, k, status, mutex, semzero;
	msg* ptr_sum;
	num_processi=m+1;

	//Forse ci va, forse no

	struct sembuf sem_buf;

	//Richiesta di spazio condiviso
	id_buffer = shmget(chiave_buffer, m*sizeof(int) + sizeof(int), IPC_CREAT|0664);

	//Collegamento memoria condivisa
	ptr_sum= (msg*) (shmat(id_buffer, 0, 0));

	//richiesta del vettore di stato ed inizializzazione
	id_stato=shmget(chiave_stato, m*sizeof(int),IPC_CREAT|0664);    //un'altra area condivisa di nome id_stato che serve a contenere lo stato
	printf("id_stato=%d \n",id_stato);
	stato=(int *) ( shmat(id_stato,0,0));
	int ci;		
					
	//inizializzazione del vettore di stato a tutti VUOTI

	for (ci=0;ci<m;ci++)							//li inizializza tutti a VUOTI per cominciare
		stato[ci]=VUOTO;

	//INIZIALIZZAZIONE DEI SEMAFORI

	mutex = semget(chiave_sem, 2, IPC_CREAT|0664);
	semctl(mutex, MUTEXP, SETVAL, 1);
	semctl(mutex, spazio_disp, SETVAL, m);

	semzero = semget(chiave_zero, 1, IPC_CREAT|0664);
	semctl(semzero, semaforzero, SETVAL, m);

	//fork per la creazione dei processi che si occuperanno delle somme
	
	for(k=0; k<num_processi; k++){
	
	pid=fork();
	
	if(pid==0){
		if(k!=num_processi-1){
			printf("Sono il figlio sommatore con pid: %d\n", getpid());
			sleep(4);
			somma_colonna(stato, ptr_sum, mutex, semzero, k, m, mat);	
			}
		else if (k = num_processi){
			printf("Sono il figlio che produce il totale: %d\n", getpid());	
			somma_totale(ptr_sum, semzero, m);
			sleep(2);
			printf("La somma totale delle somme delle colonne e': %d\n", ptr_sum[m+1]);
			}
	
	}
	
	
	}
	//processo padre
	//for (k=0; k<num_processi;k++){
	//pid=wait(&st);
	//if (pid==-1)
	//	perror("errore");
	//else
	//	 printf ("Figlio n.ro %d e\' morto con status= %d \n ",pid,st);
	//}



	// rimozione memoria condivisa e semafori
	shmctl(id_buffer,IPC_RMID,0);
	shmctl(id_stato,IPC_RMID,0);
	semctl(mutex,0,IPC_RMID);
	semctl(semzero,0,IPC_RMID);


	return 0;
}

