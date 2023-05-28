#ifndef HEADER_H
#define HEADER_H

/*
Applicazione multiprocesso. 
51 processi (50 clienti, 1 visualizzatore).
Vettore di 80 Posti.
*/

#define MUTEX 0 // semaforo init 1

#define LIBERO 0 // stato posto
#define OCCUPATO 1 // stato posto
#define UPDATING 2 // stato posto

#define NUM_POSTI 80 // num posti del teatro
#define NUM_CLIENTI 50 // num di clienti da istanziare

typedef struct {
	unsigned int id_cliente; // pid, se == 0 libero
	unsigned int stato; // stato del posto
} Posto;

typedef struct {
	Posto vet[NUM_POSTI]; // vettore di 80 posti
	unsigned int disponibilita; // contatore di posti init 80
	int sem; // id del semaforo su cui fare wait e signal
} Teatro;

int cliente(Teatro*); // int per restituire errori
void visualizzatore(Teatro*);

#endif
