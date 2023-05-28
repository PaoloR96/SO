/*
Applicazione multithread con monitor
Vettore di voli con dimensione 3 (max 3 voli contemporanei)
Produttore/Consumatore con vettore di stato
*/

#ifndef header_h
#define header_h

#include <pthread.h>

#define MAX_VOLI 3

#define NUM_THREAD 5

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

typedef struct {
	int id;
	int quota;
} Volo;

typedef struct {
	Volo vettore_voli[MAX_VOLI];
	int vettore_stato[MAX_VOLI];
	
	pthread_mutex_t mutex_vettore_stato;
	pthread_mutex_t mutex_num_voli;
	
	pthread_cond_t spazio_disp;
	pthread_cond_t volo_disp;
	
	short num_voli;
} GestioneVoli;

typedef struct {
	GestioneVoli *gv;
	int index;
} ThreadArg;

/*
PRODUTTORE. 
Inserisce un volo con quota iniziale 0 e con l'id passato.
L'inserimento dura un secondo (sleep).
Lock bloccante.
*/
void inserisci_volo(GestioneVoli *g, int id);

/*
CONSUMATORE.
Prima ricerca l'indice dell'id del volo, e poi lo cancella.
La rimozione dura un secondo (sleep).
Lock NON bloccante.
*/
void rimuovi_volo(GestioneVoli *g, int id);

/*
1) Ricerca del volo (stato == OCCUPATO)
2) Porre IN_USO
3) Modificare la quota, aggiornandola
4) Porre OCCUPATO
L'aggiornamento dura un numero random fra 1 e 3 secondi.
Lock NON bloccante.
*/
void aggiorna_volo(GestioneVoli *g, int id, int new_quota);

/*
Procedura svolta dai thread.
arg: indice del ciclo for col quale istanziamo NUM_THREAD threads.
1) Creazione di un nuovo volo, con id = arg
2) Tre aggiornamenti del suddetto volo (1000, 2000, 1000)
3) Rimozione del suddetto volo
*/
void* doit(void* arg);

#endif // header_h
