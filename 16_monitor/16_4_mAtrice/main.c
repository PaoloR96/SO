/*
 *			ESERCITAZIONE 
 *	Scrivere un’applicazione concorrente per il calcolo della
 *	somma degli elementi contenuti in una matrice.
 *
 *	I processi P_i accedono al buffer vettoreSomma per la somma
 *	della colonna i-sima in mutua esclusione senza alcun
 *	vincolo sulla posizione
 *	(e.s. Il processo P2 termina per primo e scrive
 *	nella casella vettoreSomma[0]; il processo P1 finisce per
 *	secondo e scrive nella casella vettoreSomma[1], ecc.)
 *
 *
 *	Il processo P (per la somma totale) resta in attesa
 *	della terminazione di tutti i processi P_i.
 *	Il processo padre (diverso da P) attende la
 *	terminazione di tutti i processi e restituisce a
 *	video il totale calcolato da P
 *	Implementare la soluzione memorizzando le risorse
 *	necessarie in memoria condivisa e attraverso
 *	l’utilizzo del costrutto monitor	
 *
 */

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include"./Monitor/monitor.h"
#include"mialib.h"

int main(){
	
	pid_t pid;
	int id_shm;
	int status;
	shm* memoria;
	Matrice matrice;
	int x;
	int y;
	int i;
	int k;

	printf("\n\n __INIZIO__ \n\n");

	//creo memoria condivisa e inizializzo i dati della shm
	id_shm = shmget(IPC_PRIVATE, sizeof(shm), IPC_CREAT | 0664);
	memoria = (shm*)shmat(id_shm, 0, 0);
	memoria->contatore_pos = 0;
	memoria->risultato = 0;
	for(i=0; i<DIM_X; i++){
		memoria->vettoreSomma[i] = 0;
	}
	
	//creato e inizializzato la matrice
	printf(" stampo la matrice \n");	
	srand(time(NULL));
	for(x=0; x<DIM_X; x++){
		for(y=0; y<DIM_Y; y++){
			matrice[x][y] = rand()%99;
			printf("%d\t", matrice[x][y]);
		}
		printf("\n");
	}
	
	printf("\n");

	//inizializzo il monitor
	init_monitor(&(memoria->monitor), NUM_CONDITIONS);

	//creo i processi
	for(i=0; i<DIM_Y+1; i++){
		pid = fork();
		if(pid == 0){
			if(i == 0){
				//Processo Master
				printf("Sono processo MASTER <%d>\n",getpid());
				memoria->risultato = sommaVettore(memoria);
			}else{
				//Processi per le singole somme
				printf("Sono processo <%d> numero <%d>\n",getpid(), i);
				sommaColonna(memoria, i-1, matrice);
			}
			exit(0);
		}
	}

	sleep(3);

	//attendo la terminazione dei processi 
	for(i=0; i<DIM_Y+1; i++){
		pid = wait(&status);
		printf("Pocesso <%d> TERMINATO con status <%d>\n",pid,status);
	}

	//stampo a video il vettoreSomma e il risultato finale
	printf("\n Vettore somma colonne\n");
	for(i=0; i<DIM_Y; i++){
		printf("%d\t", memoria->vettoreSomma[i]);	
	}
	printf("\n");
	printf("La somma finale è : %d\n", memoria->risultato);
	
	//rimuovo memoria condivisa e monitor
	shmctl(id_shm, IPC_RMID, 0);
	remove_monitor(&(memoria->monitor));

	printf("\n\n __INIZIO__ \n\n");

	return 0;
}
