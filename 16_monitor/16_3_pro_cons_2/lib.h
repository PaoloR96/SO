#ifndef _MIALIB__
#define _MIALIB__

//numero di processi da creare
#define NUM_PROC 6

//numero di condizioni(semafori) da passare al monitor
#define NUM_CONDITIONS 2

//semafori da creare per gestire le condition
enum{ OK_CONSUMA, OK_PRODUCI };

//memoria condivisa più variabili condition
typedef struct{
	int dato;
	int ok_consuma;
	int ok_produci;
	Monitor monitor;
}shm;

//procedure per la gestione consumatore produttore
void produttore(shm*);
void consumatore(shm*);
void inizioProduzione(shm*);
void fineProduzione(shm*);
void inizioConsumazione(shm*);
void fineConsumazione(shm*);

#endif /* _MIALIB__ */
