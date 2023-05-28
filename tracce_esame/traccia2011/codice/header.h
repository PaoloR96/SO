#ifndef HEADER_H_
#define HEADER_H_

#define NUM_SCHEDULER 1 // un solo processo scheduler
#define NUM_USERS 5 // numero di processi che fanno richiesta uso disco

#define NUM_REQUEST 5 // numero di richieste per ogni processo
#define SIZE_REQUEST 10 // grandezza del buffer circolare delle richieste 

#define SIZE_DISK 20 // posizioni in cui scrivere = [0:19]

#define NUM_VAR 2 
#define REQ_MADE 0 // cond variable per far partire l'user
#define EMPTY_SPACE 1 // cond variable per far partire lo scheduler

#define INVALID_POS 100

#include <sys/types.h> // pid_t
typedef struct {
	unsigned int posizione; // da 0 a 19 (rand)
	pid_t processo; // valore da scrivere su disco
} Richiesta;

typedef pid_t Disco[SIZE_DISK];

typedef struct {
	int head; // dove prevela lo scheduler
	int tail; // dove scrive l'utente
	
	int num_spazi_disp;
	int num_spazi_occ;
	
	Richiesta vector[SIZE_REQUEST]; // vettore di richieste
} Buffer;

#include "../monitor/monitor.h" // Monitor (Hoare)
void schedulatore(Monitor*, Buffer*, pid_t*);
void utente(Monitor*, Buffer*);

// utility
void initBuffer(Buffer*);

#endif
