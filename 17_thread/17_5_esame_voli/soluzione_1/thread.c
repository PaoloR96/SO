#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "thread.h"

//funzione ricerca di utility
static int ricerca_volo(GestioneVoli *g, int identificativo){
	int i;
	for(i=0; i<DIM; i++){
		if(g->vettore_voli[i].identificativo == identificativo ){
			//restituisce al chiamante l'indeice di dove cancellare il volo
			return i;
		}
	}
	return -1;
}

//funzione ricerca di utility
static int ricerca_volo_libero(GestioneVoli *g){
	int i;
	for(i=0; i<DIM; i++){
		if(g->vettore_stato[i] == LIBERO){
			//restituisce al chiamente l'indice di dove devo inserire il volo
			return i;
		}
	}
	return -1;	
}

//funzione ricerca di utility
static int ricerca_volo_occupato(GestioneVoli *g, int identificativo){
	int i;
	for(i=0; i<DIM; i++){
		if((g->vettore_stato[i] == OCCUPATO)&&(g->vettore_voli[i].identificativo == identificativo)){
			//restituisce al chiamante l'indice di di quale volo eliminare
			return i;
		}
	}
	return -1;
}


//procedura per l'inserimento di un volo
void InserisciVolo(GestioneVoli *g, int identificativo){

	int i;
	
	//entro nel monitor
	pthread_mutex_lock(&(g->MUTEX));

	while(g->dimensione == 0){	
		//attendo che ci sia spazio disponibile, uso il while perchè e una signal_and_continue
		pthread_cond_wait(&(g->SPAZIO_DISP),&(g->MUTEX));	
	}

	//mi ricavo l'indice del volo da inserire e lo metto IN_USO
	i = ricerca_volo_libero(g);
	g->vettore_stato[i] = IN_USO;
	//decremento la dimensione disponibile
	g->dimensione--;

	//esco dal monitor
	pthread_mutex_unlock(&(g->MUTEX));

	//apporto le modifiche nell'indice da prima calcolato
	g->vettore_voli[i].identificativo = identificativo;
	g->vettore_voli[i].quota = 0;
	printf("Volo identificativo [%d] inserito,  quota %d\n", g->vettore_voli[i].identificativo, g->vettore_voli[i].quota);
	sleep(1);
	
	//entro nel monitor
	pthread_mutex_lock(&(g->MUTEX));

	//metto lo stato in quell'indice a OCCUPATO
	g->vettore_stato[i] = OCCUPATO;

	//esco dal monitor
	pthread_mutex_unlock(&(g->MUTEX));
}

//procedura per la rimozione di un volo
void RimuoviVolo(GestioneVoli *g, int identificativo){

	int i;

	//entro nel monitor
	pthread_mutex_lock(&(g->MUTEX));

	//mi calcolo l'indice del volo da cancellare e lo metto IN_USO
	i = ricerca_volo_occupato(g,identificativo);
	g->vettore_stato[i] = IN_USO;

	//esco dal monitor
	pthread_mutex_unlock(&(g->MUTEX));

	//rimuovo il volo nell'indice calcolato in precedenza
	printf("Volo identificativo [%d] eliminato \n", g->vettore_voli[i].identificativo);
	g->vettore_voli[i].identificativo = 0;
	g->vettore_voli[i].quota = 0;
	sleep(1);
	
	//entro nel monitor
	pthread_mutex_lock(&(g->MUTEX));

	//incremento la dimensione disponibile, metto lo stato dell'indice a LIBERO, e segnalo che c'è disponibile
	g->dimensione++;
	g->vettore_stato[i] = LIBERO;
	pthread_cond_signal(&(g->SPAZIO_DISP));

	//esco dal monitor
	pthread_mutex_unlock(&(g->MUTEX));
}

//procedura per l'aggiornamento di un volo
void AggiornaVolo(GestioneVoli *g, int identificativo, int quota){
	
	int i;	
	int delay = 0;

	//entro nel monitor
	pthread_mutex_lock(&(g->MUTEX));

	//calcolo indice del volo da aggiornare, e metto la posizione IN_USO
	i = ricerca_volo(g,identificativo);
	g->vettore_stato[i] = IN_USO;
	
	//esco dal monitor
	pthread_mutex_unlock(&(g->MUTEX));

	//modifico la quota del volo
	g->vettore_voli[i].quota = quota;
	printf("Volo identificativo [%d] quota aggiornata %d\n",g->vettore_voli[i].identificativo, g->vettore_voli[i].quota);
	sleep(1+rand()%3);
	
	//entro nel monitor	
	pthread_mutex_lock(&(g->MUTEX));

	//metto la posizione a OCCUPATO
	g->vettore_stato[i] = OCCUPATO;

	//esco dal monitor
	pthread_mutex_unlock(&(g->MUTEX));
	
}

//procedura chiamata dai threads
void *Aeroporto( void *param){
	int identificativo = (int)(long)param;

	InserisciVolo(g,identificativo);
	AggiornaVolo(g,identificativo,1000);
	AggiornaVolo(g,identificativo,2000);
	AggiornaVolo(g,identificativo,1000);
	RimuoviVolo(g,identificativo);
	
	pthread_exit(NULL);
}
