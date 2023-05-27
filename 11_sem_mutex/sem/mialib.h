#ifndef __MIALIB__
#define __MIALIB__

#define MUTEX 0

typedef struct{
	long valore;
	int modificatoDa;
}msg;

void waitSem(int id_semaforo, int num_semaforo);
void signalSem(int id_semaforo, int num_semaforo);
void usoRisorsa(msg* shm_mem);

#endif
