#include <stdio.h>
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


void scrivi_matrice(int n, int m, int mat[][dmax2]){
	
	int i, j;
 
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			printf("Inserisci l'elemento [%d]x[%d]: ", i, j);
			scanf("%d", &mat[i][j]);
		}
	}

}

void leggi_matrice(int n, int m, int mat[][dmax2]){

	int i, j;
	
	printf("La matrice bella è cuesta: \n");
	
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}

}

//Procedure per la sincronizzazione sui semafori

void Wait_Sem(int id_sem, int numsem)     {
       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;
       semop(id_sem,&sem_buf,1);   //semaforo rosso
}

void Signal_Sem (int id_sem, int numsem)     {
       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;
       semop(id_sem,&sem_buf,1);   //semaforo verde
}

void WaitForZero_Sem(int id_sem, int numsem)     {
       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=0;
       semop(id_sem,&sem_buf,1);   //semaforo rosso for zero
}

int leggi_valore(int id_sem, int semaforo) {
  return semctl(id_sem, semaforo, GETVAL);
}

void somma_colonna(int* stato, msg* buffer, int mutex, int semzero, int k, int m, int mat[][dmax2]){
	int indice = RichiestaS (stato, mutex, m);
	int somma = 0;
	printf ("Somma della colonna numero %d, indice = %d, valore semaforo spazio_disp = %d \n", k, indice, leggi_valore(mutex, spazio_disp));
	int i;
	
	for(i = 0; i < m; i++)
		somma += mat[i][indice];
	
	Sommatoria(indice, buffer, mat, m, somma);

	sleep(2);  // simula un produttore "lento ma non tanto" (il primo, con indice 0)
	//printf("La somma della colonna %d è: %d", indice, buffer[indice]);

	RilascioS(indice,stato,mutex);
}

int RichiestaS (int*stato, int mutex, int m){
	int indice=0;
	Wait_Sem(mutex, spazio_disp); // Attende che vi sia spazio per produrre
	Wait_Sem(mutex, MUTEXP); // Blocca eventuali altri produttori
	// determina l'indice del primo elemento VUOTO nel buffer
	while ((indice <= m)&&(stato[indice] != VUOTO))
		indice++;
	stato[indice] = IN_USO; // segna la cella come in uso
	Signal_Sem(mutex, MUTEXP); //Sblocca eventuali altri produttori
	return indice;
}

void Sommatoria(int indice, msg *buffer, int mat[][dmax2], int m, int somma){
	//effettua la vera e propria somma
	buffer[indice] = somma;
	//indicecolonna++;
}

void RilascioS (int indice,int*stato,int semzero){
	stato[indice]=PIENO;  //segnala che è stato prodotto un messaggio nella cella indice
	Signal_Sem(semzero, semaforzero);  //dà il via libera AL SOMMATORE FINALE
}

void somma_totale(msg* buffer, int semzero, int m){
	Wait_Sem(semzero, semaforzero);

	int i, somma;

	for(i = 0; i < m; i++)
		somma += buffer[i];

	buffer[m+1] = somma;
}
