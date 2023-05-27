#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#define PROD  0
#define CONS  1
#define MUTEX 2

typedef struct{

	int v_min[10];

}Buffer;

int Ricerca_Minimo_Figlio (int *, int);
int Ricerca_Minimo_Buffer (Buffer *b);

void Produzione_Minimo (Buffer *, int *, int, int);
void Consumo_Minimo (Buffer *, int);


void Signal_Sem (int, int);
void Wait_Sem (int, int);


void Leggi_Buffer (Buffer *);
