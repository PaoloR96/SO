#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "header.h"

/*
Nel negozio sono disponibili 6 DVD, rappresentati da un vettore di strutture “DVD”. Ad ogni DVD del negozio è assegnato un intero “identificativo_film” che rappresenta il film contenuto nel DVD (si supponga che ci siano 3 film disponibili, identificati da un numero tra 1 e 3). Inoltre, per uno stesso film possono essere disponibili più copie, ed ogni copia ha un intero “identificativo_copia” che la distingue (si supponga che per ogni film di siano 2 copie, identificate da un numero pari a 1 o 2). Ad esempio, le 2 copie del film “1” avranno un “identificativo_copia” rispettivamente uguale a “1” e “2”; le 2 copie del film “2” avranno “identificativo_copia” uguale a “1” e “2”; etc.
*/

/*
Il metodo “affitta” accetta in ingresso l’identificativo del film da affittare. Esso cerca una copia del film disponibile, e se la trova la pone nello stato AFFITTATO. Se non la trova, il thread richiedente è messo in attesa finché non si rende disponibile una copia di quello specifico film. Il metodo deve ritornare lo identificativo_copia del DVD selezionato.
*/

int affitta(Monitor * m, int id_film){
	pthread_mutex_lock(&m->MUTEX);
	
	int i, k = 0;
	int indici[2];

	for(i = 0; i < 6; i++){
		if(m->dvd[i].identificativo_film == id_film){
			indici[k] = i;
			k++;
		}
	}
	
	while(m->dvd[indici[0]].stato == AFFITTATO && m->dvd[indici[1]].stato == AFFITTATO){
		if(id_film == 1)
			pthread_cond_wait(&m->FILM1, &m->MUTEX);
		else if(id_film == 2)
			pthread_cond_wait(&m->FILM2, &m->MUTEX);
		else
			pthread_cond_wait(&m->FILM3, &m->MUTEX);
	}

	if(m->dvd[indici[0]].stato == DISPONIBILE){
		m->dvd[indici[0]].stato = AFFITTATO;
		printf("Affittata la copia 1 del DVD: %d.\n", id_film);
		pthread_mutex_unlock(&m->MUTEX);

		return m->dvd[indici[0]].identificativo_copia;
	} else if(m->dvd[indici[1]].stato == DISPONIBILE){
		m->dvd[indici[1]].stato = AFFITTATO;
		printf("Affittata la copia 2 del DVD: %d.\n", id_film);
		pthread_mutex_unlock(&m->MUTEX);

		return m->dvd[indici[1]].identificativo_copia;
	}
}

/*
Il metodo “restituisci” accetta in ingresso l’identificativo del film e della copia presa in affitto, la pone nello stato DISPONIBILE, e rimuove dalla attesa un eventuale thread in attesa di quel film, permettendogli di affittarlo.
*/

void restituisci(Monitor *m, int id_film, int id_copia){
	pthread_mutex_lock(&m->MUTEX);
	int i;
	for(i = 0; i < 6; i++){
		if(m->dvd[i].identificativo_film == id_film && m->dvd[i].identificativo_copia == id_copia){
			m->dvd[i].stato = DISPONIBILE;
			printf("Restituita una copia del DVD: %d.\n", id_film);
			if(id_film == 1)
				pthread_cond_signal(&m->FILM1);
			else if(id_film == 2)
				pthread_cond_signal(&m->FILM2);
			else
				pthread_cond_signal(&m->FILM3);
		}
	}
	
	pthread_mutex_unlock(&m->MUTEX);
}

//Il programma deve inoltre istanziare un quinto thread che ogni secondo richiama il metodo “stampa” che mostra lo stato dei DVD del negozio.

void stampa(Monitor * m){
	pthread_mutex_lock(&m->MUTEX);
	
	int i;
	for(i = 0; i < 6; i++)
		printf("Il DVD in posizione: %d\tcon id: %d\ted identificativo copia: %d\tha stato: %d.\n", i, m->dvd[i].identificativo_film, m->dvd[i].identificativo_copia, m->dvd[i].stato);

	pthread_mutex_unlock(&m->MUTEX);
}
