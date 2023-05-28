#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>

#include "header.h"

void generatore(int msgqid){
	// create Message variable
	Message m;
	
	// set Message variables
	m.type = P_GEN;

	for(int i = 0; i < N; ++i){
		// generate random value
		double val = rand() % 100;
	
		// assign to message
		m.value = val;
	
		// send the message
		msgsnd(msgqid, (void*)&m, sizeof(m) - sizeof(long), 0);
		
		#ifdef DEBUG
		printf("Messaggio spedito: %lf\n", m.value);
		#endif
	}
}

double mediatore(int msgqid){
	// receive N messages
	double array[N];
	
	// create a Message variable where to store messages received
	Message m;
	
	for(int i = 0; i < N; ++i){
		// receive message and fill variable
		msgrcv(msgqid, (void*)&m, sizeof(m) - sizeof(long), P_GEN, 0);
		
		// store in array our message
		array[i] = m.value;
		
		#ifdef DEBUG
		printf("Valore ricevuto: %lf\n", m.value);
		#endif
	}
	
	// now my array is full
	// I have to do average
	double avg = 0;
	
	for(int i = 0; i < N; ++i){
		avg += array[i];
	}
	
	avg /= N;
	
	return avg;
}

