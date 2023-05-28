#include "header.h"
#include "../monitor/monitor.h"
#include <stdlib.h> // rand
#include <stdio.h> // printf
#include <unistd.h> // sleep, getpid

/* Utility da chiamare nel main */
void initBuffer(Buffer* b){
	Richiesta r;
	r.posizione = INVALID_POS;
	r.processo = 0;
	
	for(int i = 0; i < SIZE_REQUEST; ++i){
		b->vector[i] = r;
	}
	
	b->head = 0;
	b->tail = 0;
	
	b->num_spazi_disp = SIZE_REQUEST;
	b->num_spazi_occ = 0;
}

/* Supporto per schedulatore() e utente() */
void inizioScheduler(Monitor *mPtr, Buffer *bPtr){
	enter_monitor(mPtr);
	
	if(bPtr->num_spazi_occ == 0)
		wait_condition(mPtr, REQ_MADE);
	
	leave_monitor(mPtr);
}

void fineScheduler(Monitor *mPtr, Buffer *bPtr){
	enter_monitor(mPtr);
	
	bPtr->num_spazi_occ -= 1; // ho consumato uno spazio
	bPtr->num_spazi_disp += 1; // e ho liberato uno spazio
	
	if(bPtr->num_spazi_occ == 0)
		signal_condition(mPtr, EMPTY_SPACE); // sblocca tutti i processi bloccati sulla wait
	
	leave_monitor(mPtr);
}

void inizioUtente(Monitor *mPtr, Buffer *bPtr){
	enter_monitor(mPtr);
	
	if(bPtr->num_spazi_disp == 0)
		wait_condition(mPtr, EMPTY_SPACE);
	
	bPtr->num_spazi_disp -= 1;
	bPtr->num_spazi_occ += 1;
	
	leave_monitor(mPtr);
}

void fineUtente(Monitor *mPtr, Buffer *bPtr){
	enter_monitor(mPtr);
	
	signal_condition(mPtr, REQ_MADE);
	
	leave_monitor(mPtr);
}

int getHead(Monitor *mPtr, Buffer *bPtr){
	enter_monitor(mPtr);
	
	int head = bPtr->head;
	bPtr->head += 1;
	if(bPtr->head == SIZE_REQUEST) {
		bPtr->head = 0;
	}
	
	leave_monitor(mPtr);

	return head;
}

int getTail(Monitor *mPtr, Buffer *bPtr){
	enter_monitor(mPtr);
		
	int tail = bPtr->tail;
	bPtr->tail += 1;
	if(bPtr->tail == SIZE_REQUEST) {
		bPtr->tail = 0;
	}
	
	leave_monitor(mPtr);
	
	return tail;
}

/* Funzioni da chiamare nel main */
void schedulatore(Monitor *mPtr, Buffer *bPtr, pid_t *d){
	for(int i = 0; i < NUM_USERS * NUM_REQUEST; ++i){
		inizioScheduler(mPtr, bPtr);
		
		int pos = getHead(mPtr, bPtr);
	
		Richiesta r = bPtr->vector[ pos ];
	
		int oldPos;
		if(pos == 0)
			oldPos = SIZE_REQUEST - 1;
		else
			oldPos = pos - 1;

		Richiesta rOld = bPtr->vector[ oldPos ];
	
		int t = r.posizione;
		// stiamo accedendo a un elemento sporco perché è la prima richiesta che serviamo, quindi lo "aggiustiamo"
		if( rOld.posizione == INVALID_POS ){
			t -= 0;
		} else {
			t -= rOld.posizione;
		}

		// fare una sleep secondo quanto descritto nel pdf
		if(t < 0)	t *= -1; // abs(t)
	
		printf("SCHEDULER: Sto per avviare la procedura su disco... Attendere %d secondi\n", t);
		//sleep(t);
		//sleep(1);
	
		// scrivere in r.posizione il valore r.processo
		d[r.posizione] = r.processo;
		
		printf("SCHEDULER: Ho scritto %d sul disco (posizione %d)\n", d[r.posizione], r.posizione);
		
		fineScheduler(mPtr, bPtr);
	}
}

void utente(Monitor *mPtr, Buffer *bPtr){
	for(int i = 0; i < NUM_REQUEST; ++i) {
		inizioUtente(mPtr, bPtr);
		
		int pos = getTail(mPtr, bPtr);
	
		pid_t myPid = getpid();
		int posizione = rand() % 20;
	
		Richiesta r;
		r.posizione = posizione;
		r.processo = myPid;
	
		bPtr->vector[pos] = r;
		
		printf("UTENTE %d: Ho messo la mia richiesta <posizione: %d, processo: %d> nel buffer in posizione %d\n", getpid(), r.posizione, r.processo, pos);
		
		fineUtente(mPtr, bPtr);
	}
}
