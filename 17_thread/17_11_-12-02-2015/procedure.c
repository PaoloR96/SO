#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include "header.h"

int InizioProd(MonitorElaborazioni * ME){
	int indice = 0;
	pthread_mutex_lock(&(ME->MUTEX));

	while(ME->cellevuote == 0)
		pthread_cond_wait(&(ME->ok_produzione), &(ME->MUTEX));

	while((indice < 4)&&(ME->VettStato[indice] != VUOTO))
		indice++;

	ME->VettStato[indice] = IN_USO;

	return indice;
}

void FineProd(MonitorElaborazioni* ME, int indice){
	ME->VettStato[indice] = PIENO;
	ME->cellepiene++;
	ME->cellevuote--;
	pthread_cond_signal(&(ME->ok_consumo));
	pthread_mutex_unlock(&(ME->MUTEX));
}

void *Produttore(void* arg){
	MonitorElaborazioni *ME = (MonitorElaborazioni *) arg;
	int i;
	
	for(i = 0; i < 3; i++){
		srand(time(0));
		int val = 2 + rand() % 3;
		int k;
		int indice = InizioProd(ME);		//richiesta

		(ME->elaborazioni[indice]).totale_operandi = val;

		printf("Ho prodotto nella posizione %d del vettore ELABORAZIONI: ", indice);	
		for(k = 0; k < val; k++){
			(ME->elaborazioni[indice]).operandi[k] = rand() % 11;
			printf("%d ", (ME->elaborazioni[indice]).operandi[k]);
		}
		printf("\n");

		sleep(val);

		FineProd(ME, indice);
	}
	
	pthread_exit(NULL);
}

int InizioCons(MonitorElaborazioni* ME){
	//int minore = 0;
	//int k = 0;
	pthread_mutex_lock(&(ME->MUTEX));

	while(ME->cellepiene == 0)
		pthread_cond_wait(&(ME->ok_consumo), &(ME->MUTEX));

	/*while((k < 4)&&(ME->VettStato[k] == PIENO)){
		if((ME->elaborazioni[k]).totale_operandi < (ME->elaborazioni[k+1]).totale_operandi)
			minore = k;
		k++;
	}*/

	/*for(k = 0; k < 4; k++){
		if((ME->VettStato[k] == PIENO)&&(ME->VettStato[k+1] == PIENO)){
			if((ME->elaborazioni[k]).totale_operandi < (ME->elaborazioni[k+1]).totale_operandi)
				minore = k;		
		}
	}	
	*/

	int i = 0;

	int minimo = -1;
	int operandi_minimo = -1;


	for(i=0; i<5; i++) {

		if(ME->VettStato[i] == PIENO  &&  (minimo==-1  ||  ME->elaborazioni[i].totale_operandi < operandi_minimo) ) {

			minimo = i;
			operandi_minimo = ME->elaborazioni[i].totale_operandi;
		}
	}


	return minimo;
	//ME->VettStato[minore] = IN_USO;

	//return minore;
}


void FineCons(MonitorElaborazioni* ME, int indice){
	ME->VettStato[indice] = VUOTO;
	ME->cellepiene--;
	ME->cellevuote++;
	pthread_cond_signal(&(ME->ok_produzione));
	pthread_mutex_unlock(&(ME->MUTEX));
}

void *Consumatore(void* arg){
	MonitorElaborazioni* ME = (MonitorElaborazioni*) arg;
	int j;

	for(j = 0; j < 6; j++){
		int k;
		int somma = 0;
		int minore = InizioCons(ME);

		//Cerca la cella con meno operandi nel buffer
	
		printf("Consumo il valore del buffer %d: ", minore);
		for(k = 0; k < (ME->elaborazioni[minore]).totale_operandi; k++){
			printf("%d ", (ME->elaborazioni[minore]).operandi[k]);
			somma += (ME->elaborazioni[minore]).operandi[k];
		}
	
		printf("\nLa somma degli operandi è: %d\n", somma);

		sleep(2);
		FineCons(ME, minore);
	}

	pthread_exit(NULL);
}
