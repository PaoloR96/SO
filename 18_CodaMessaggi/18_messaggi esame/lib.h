#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/ipc.h>

#define TERMINA 1
#define RICHIESTA 2
#define DIM sizeof(messaggio)-sizeof(long)

typedef struct{
	long tipo;
	int a;
	int b;
	int prodotto;
	pid_t pid;
}messaggio;

typedef struct{
	messaggio vet[15];
	int riempimento;
	int testa;
	int coda;
	pthread_cond_t OKPROD;
	pthread_cond_t OKCONS;
	pthread_mutex_t MUTEX;
}array_cond;
extern pthread_t pthread[3];
extern int queue_ric,queue_risp;

#endif