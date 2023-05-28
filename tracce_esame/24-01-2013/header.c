#include "header.h"
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

typedef struct {
	int first;
	int second;
} twoInts;

twoInts trovaFilm(Monitor *mPtr, int id_film){
	twoInts t;
	int j = 0;
	
	for(int i = 0; i < NUM_DVD; ++i){
		if( mPtr->dvd[i].id_film == id_film ){
			if( j == 0 )
				t.first = i;
			else
				t.second = i;
				
			j++;
		}
	}
	
	return t;
}

int trovaCopia(Monitor *mPtr, int id_film, int id_copia){
	twoInts t = trovaFilm(mPtr, id_film);
	
	DVD *d1 = &mPtr->dvd[t.first];
	DVD *d2 = &mPtr->dvd[t.second];
	
	/*printf("DENTRO TROVA COPIA\n");
	printf("t.first: %d\nt.second: %d\n", t.first, t.second);
	printf("id_copia: %d\n", id_copia);
	
	printf("d1.id_copia: %d\nd2.id_copia: %d\n", (*d1).id_copia, (*d2).id_copia);*/
	
	if( (*d1).id_copia == id_copia ) return t.first;
	else if ( (*d2).id_copia == id_copia ) return t.second;
	else return -1;
}

int affitta(Monitor *mPtr, int id_film){
	/* trova le occorrenze del film ricercato */
	twoInts t; // due copie per film
	
	pthread_mutex_lock( &mPtr->mutex );

	t = trovaFilm(mPtr, id_film);
	
	/* ora che ho le occorrenze, devo fare un while-wait */
	printf("%ld\nAspetto che almeno una copia di %d sia disponibile\n", pthread_self(), id_film);
	while( mPtr->dvd[t.first].stato == AFFITTATO && mPtr->dvd[t.second].stato == AFFITTATO )
		pthread_cond_wait( &mPtr->disponibile, &mPtr->mutex );
	
	printf("%ld\nAlmeno una copia è disponibile, allora me la prendo io!\n", pthread_self());
	/* se sono qui, almeno uno dei due è DISPONIBILE */
	int ret; // indice del DVD impostato su AFFITTATO
	if( mPtr->dvd[t.first].stato == DISPONIBILE )
		ret = mPtr->dvd[t.first].id_copia;
	else
		ret = mPtr->dvd[t.second].id_copia;
	
	/* cambio lo stato del DVD preso e restituisco l'indice al programma chiamante */
	mPtr->dvd[ret].stato = AFFITTATO;
	printf("%ld\nL'indice %d e' stato affittato da me\n", pthread_self(), ret);
	
	pthread_mutex_unlock( &mPtr->mutex );
	
	return ret;
}

void restituisci(Monitor *mPtr, int id_film, int id_copia){
	pthread_mutex_lock( &mPtr->mutex );
	
	int occ = trovaCopia(mPtr, id_film, id_copia);
	if( occ < 0 ){
		fprintf(stderr, "FATAL ERROR\n");
		exit(0);
	}
	
	if( mPtr->dvd[occ].stato == DISPONIBILE ){
		//fprintf(stderr, "Il DVD era stato gia' restituito!\n");
	} else {
		mPtr->dvd[occ].stato = DISPONIBILE; /* era affittato, e torna disponibile */
	}
	
	printf("%ld\nHo restituito il dvd %d (copia %d) (indice %d)\n", pthread_self(), id_film, id_copia, occ);
	
	pthread_cond_broadcast( &mPtr->disponibile );
	pthread_mutex_unlock( &mPtr->mutex );
}

void stampa(Monitor *mPtr){
	char disp[] = "DISPONIBILE";
	char aff[] = "AFFITTATO";
	char stato[15];
	
	pthread_mutex_lock( &mPtr->mutex );
	
	for(int i = 0; i < NUM_DVD; ++i){
		DVD d = mPtr->dvd[i];
		
		if( d.stato == DISPONIBILE )
			strcpy(stato, disp);
		else
			strcpy(stato, aff);
		
		printf("Stato del DVD %d (copia %d): %s\n", d.id_film, d.id_copia, stato);
	}
	
	pthread_mutex_unlock( &mPtr->mutex );
}

void inizializza(Monitor *mPtr){
	pthread_mutex_init( &mPtr->mutex, NULL );
	pthread_cond_init( &mPtr->disponibile, NULL );
	
	int id_film = rand() % 3 + 1;
	int id_copia = rand() % 2 + 1;
	DVD d;
	
	for(int i = 0; i < NUM_DVD; ++i){
		d.id_film = id_film;
		id_film = (id_film + 1) % 4;
		if(id_film == 0) id_film++;
		
		d.id_copia = 0;
		
		d.stato = DISPONIBILE;
			
		mPtr->dvd[i] = d;
	}
	
	for(int i = 0; i < NUM_DVD; ++i){
		twoInts t = trovaFilm(mPtr, mPtr->dvd[i].id_film);
		
		mPtr->dvd[t.first].id_copia = 1;
		mPtr->dvd[t.second].id_copia = 2;
	}
}
