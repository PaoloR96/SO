#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include"./Monitor/monitor.h"
#include"mialib.h"

//fa la somma della colonna della matrice passata in input e la salva nella memoria condivisa
void sommaColonna(shm *memoria, int colonna, Matrice matrice){
	
	//dichiarazione variabili interne	
	int x;
	int somma = 0;

	//faccio la somma della colonna che passo come parametro
	for(x=0; x<DIM_Y; x++){
		somma = (somma + matrice[x][colonna]);	
	}	
	printf("\n");
	printf("la somma della colonna <%d> è : <%d>\n", colonna, somma);

	//entro nel monitor
	enter_monitor(&(memoria->monitor));

	//salvo la somma nel vettoe nella posizione contatore che mi tiene il conteggio della posizione corrente
	memoria->vettoreSomma[memoria->contatore_pos] = somma;
	memoria->contatore_pos++;

	//quando il contatore ha raggiunto la dimensione dell'array, avverto al master tramite FINITO_DI_SOMMARE che tocca a lui
	if(memoria->contatore_pos == DIM_Y ){
		signal_condition(&(memoria->monitor), FINITO_DI_SOMMARE);
	}

	//esco dal monitor
	leave_monitor(&(memoria->monitor));	
}

//viene eseguito da master una volta che gli altri processi hanno terminato di fare le somme delle colonne
int sommaVettore(shm* memoria){
	
	//dichiarazione delle variabili	
	int i;
	int risultato = 0;

	//entro nel monitor
	enter_monitor(&(memoria->monitor));

	//devo attendere che i processi facciano le somme e riempiano il vettore
	wait_condition(&(memoria->monitor), FINITO_DI_SOMMARE);
	
	//faccio la somma
	for(i=0; i<DIM_Y; i++){
		risultato = risultato + memoria->vettoreSomma[i];
	}
	
	//abbandono il monitor
	leave_monitor(&(memoria->monitor));

	//faccio tornare il risultato al programma chiamante
	return risultato;
}
