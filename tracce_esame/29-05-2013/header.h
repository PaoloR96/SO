/*   T R A C C I A 
Si realizzi in linguaggio C/C++ un processo servente multithread basato su code di messaggi UNIX.
Il processo servente, denominato Server, riceve richieste di elaborazione provenienti da un gruppo di 3
processi denominati Client. Ogni client invia 5 richieste di elaborazione. Ogni messaggio inviato dovrà
contenere il PID del Client e due valori interi, selezionati casualmente tra 0 e 10 con la funzione
rand(). I processi Client devono inoltre attendere e stampare a video il messaggio di risposta del
Server proveniente da un’apposita coda dei messaggi di risposta, prima di mandare la richiesta
successiva. Il codice dei Client e del Server deve risiedere in due eseguibili distinti, che sono invocati
da un programma principale attraverso una delle varianti della primitiva exec. Quando tutti i Client
terminano, il programma principale attende 3 secondi e poi invia un messaggio speciale al Server,
contenente la coppia di valori {-1, -1}, che causa la terminazione del Server come illustrato di seguito.
Il Server istanzia 2 tipi di thread, rispettivamente un Manager e 2 Worker. Il Manager verifica
periodicamente la disponibilità di un messaggio senza bloccarsi, attendendo con una sleep di un
secondo tra un controllo e l'altro. Quando riceve un messaggio, lo pone in un array di buffer condiviso
con i thread Worker. Il messaggio viene prelevato da un thread Worker, che calcola il prodotto della
coppia di valori, ed ha il compito di inviare al Client (identificato dal PID) un messaggio contenente il
prodotto risultante, tramite la coda di messaggi di risposta. Quando il Manager riceve il messaggio con
la coppia di valori {-1,-1}, forza la terminazione dei Worker con pthread_cancel e termina a sua
volta. Il processo Server attende la terminazione del thread Manager per poi terminare.
*/

#ifndef HEADER
#define HEADER

#include <sys/types.h>
#include <pthread.h>

#define NUM_PROC_SERVER 1
#define NUM_PROC_CLIENT 3
#define NUM_PROC NUM_PROC_SERVER+NUM_PROC_CLIENT

#define NUM_REQ 5
#define SIZE_BUFF 10

#define NUM_THREAD_MANAGER 1
#define NUM_THREAD_WORKER 2
#define NUM_THREAD NUM_THREAD_MANAGER+NUM_THREAD_WORKER

#define PATH_Q "."
#define KEY_Q 'k'

typedef struct {
	long type; // tipo del messaggio
	pid_t pid; // pid del client
	int n1; // rand fra 0 e 10
	int n2; // rand fra 0 e 10
} MessageCS; // from client to server

typedef struct {
	long type;
	int result;
} MessageSC; // from server to client

#define EMPTY 0
#define FULL 1
#define USING 2

typedef struct {
	MessageCS array[SIZE_BUFF];
	int state[SIZE_BUFF];
	
	pthread_mutex_t mutex;
	pthread_cond_t SPAZIO_DISP;
	pthread_cond_t MSG_DISP;
	
	int server_qid;
} Buffer;

typedef struct {
	pthread_t *w_threads;
	
	Buffer *bPtr;
} Manager;

void client(int server_queue_id);
void server(int server_queue_id);

void* manager(void* arg);
void* worker(void* arg);

#endif
