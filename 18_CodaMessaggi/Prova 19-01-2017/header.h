#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>

#define LTT 1
#define ACQ 2
#define FRN 3

typedef struct {
	int costo;
	int quantita;
}Magazzino [10];

typedef struct {
	
	long tipo;
	pid_t pid;
	
	int prodotto;
	int quantita;
}Messaggio;

typedef struct {
	long tipo;
	
	int costo[10];
	int quantita[10];
}Risposta;


typedef struct {
	long tipo;
	
	int prodotto;
	int quantita;
}RispostaAcquisto;

void InizializzaMagazzino (Magazzino *);
void Client (int, int);
void Fornitore (int, int);
void Gestore (Magazzino *, int, int);
