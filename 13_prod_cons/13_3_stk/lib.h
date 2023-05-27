#ifndef __HELLO__
#define __HELLO__

//semafori
#define MSG_DISP 0
#define SPAZIO_DISP 1
#define MUTEX_PROD 2
#define MUTEX_CONS 3

//dimensione pila
#define DIMENSIONE 5

//numero processi che il programma deve generare
#define NUM_PROCESSI 4

//numero di consumazioni e produzioni per processo
#define NUM_PROD 2
#define NUM_CONS 2

typedef struct {
	int vettore[DIMENSIONE];
	int testa;
}Pila;

//funzioni gestione semaforo
void signal(int,int);
void wait(int,int);

//funzioni gestione produttore e consumatore
void produttore(int,Pila*);
void consumatore(int,Pila*);

#endif /* __HELLO__ */
