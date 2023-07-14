/*
	Produttori consumatori con un vettore di stato
*/
#include <stdio.h> // printf, scanf
#include <stdlib.h> // exit, NULL
#include <unistd.h> // fork
#include <wait.h> // wait
#include <sys/types.h> // pid_t, key_t
#include <sys/ipc.h> // IPC macros
#include <sys/shm.h> // shmget, shmctl
#include <sys/sem.h> // semget, semctl

#include "semaphore.h" // waitSem, signalSem

/// SHARED MEMORY
const unsigned short int SIZE = 20; // dimension of buffer and status vector
const unsigned short int EMPTY = 0, USING = 1, FULL = 2;
typedef int type;

/// SEMAPHORE
const unsigned short int SPAZIO_DISP = 0, MSG_DISP = 1, MUTEXP = 2, MUTEXC = 3;

/// FUNCTIONS
void produttore(int sem_id, type *bufferPtr, int *vectorPtr, type product){
	waitSem(sem_id, SPAZIO_DISP); // INIZIO RICERCA SPAZIO
	// una volta entrato, ho sicuramente lo spazio
	// uso il mutex solo per prendermi uno slot
	waitSem(sem_id, MUTEXP); // INIZIO CATTURA SLOT
	// qui entra uno alla volta, quindi mi prendo uno slot
	// sono sicuro di riempire freeSlot, perché ho fatto la wait sullo SPAZIO_DISP
	int freeSlot;
	for(int i = 0; i < SIZE; ++i){
		if( *(vectorPtr + i) == EMPTY ){
			freeSlot = i;
			break;
		}
	}
	*(vectorPtr + freeSlot) = USING;
	signalSem(sem_id, MUTEXP); // FINE CATTURA SLOT
	// segnalo al prossimo produttore che può cercare uno slot libero
	// il mio l'ho preso

	*(bufferPtr + freeSlot) = product;
	*(vectorPtr + freeSlot) = FULL;

	signalSem(sem_id, MSG_DISP); // SEGNALAZIONE MSG PRESENTE
}

type consumatore(int sem_id, type *bufferPtr, int *vectorPtr){
	waitSem(sem_id, MSG_DISP);
	waitSem(sem_id, MUTEXC);

	int fullSlot;
	for(int i = 0; i < SIZE; ++i){
		if( *(vectorPtr + i) == FULL ){
			fullSlot = i;
			break;
		}
	}
	*(vectorPtr + fullSlot) = USING;

	signalSem(sem_id, MUTEXC);
	// Ho trovato uno slot pieno, ho posto il suo valore di vettore a USING,
	// e ora posso consumarlo

	type t = *(bufferPtr + fullSlot);
	*(vectorPtr + fullSlot) = EMPTY;

	signalSem(sem_id, SPAZIO_DISP);

	return t;
}

int main(){
	key_t shmBufferKey = ftok(".", 'B');
	key_t shmVectorKey = ftok(".", 'V');
	key_t semKey = ftok(".", 'S');

	int shmBufferID = shmget(shmBufferKey, sizeof(type) * SIZE, IPC_CREAT|IPC_EXCL|0664);
	if(shmBufferID < 0){
		// se l'area di memoria già esiste, mi ci collego e basta
		shmBufferID = shmget(shmBufferKey, sizeof(type) * SIZE, 0664);
		if(shmBufferID < 0){
			fprintf(stderr, "FATAL ERROR! Cannot istantiate shared memory for buffer\n");
			exit(1);
		}
	}

	int shmVectorID = shmget(shmVectorKey, sizeof(int) * SIZE, IPC_CREAT|IPC_EXCL|0664);
	if(shmVectorID < 0){
		// se l'area di memoria già esiste, mi ci collego e basta
		shmVectorID = shmget(shmVectorKey, sizeof(int) * SIZE, 0664);
		if(shmVectorID < 0){
			fprintf(stderr, "FATAL ERROR! Cannot istantiate shared memory for vector\n");
			exit(1);
		}
	}

	int semID = semget(semKey, 4, IPC_CREAT|0664);
	if(semID < 0){
		fprintf(stderr, "FATAL ERROR! Cannot istantiate semaphore\n");
		exit(2);
	}

	/*
	Se sono arrivato qui, ho gli ID di shmBuffer, shmVector, sem.
	Ora devo settarli uno per uno:
	Faccio l'attach delle due shared memory;
	Setto i valori dei semafori.
	*/

	type *bufferPtr;
	int *vectorPtr;

	bufferPtr = (type*)shmat(shmBufferID, 0, 0);
	if(bufferPtr == (void*)-1){
		fprintf(stderr, "FATAL ERROR! Cannot attach shared memory to bufferPtr\n");
		exit(3);
	}

	vectorPtr = (int*)shmat(shmVectorID, 0, 0);
	if(vectorPtr == (void*)-1){
		fprintf(stderr, "FATAL ERROR! Cannot attach shared memory to vectorPtr\n");
		exit(4);
	}

	for(int i = 0; i < SIZE; ++i){
		*(vectorPtr + i) = EMPTY;
	}

	semctl(semID, SPAZIO_DISP, SETVAL, SIZE);
	semctl(semID, MSG_DISP, SETVAL, 0);
	semctl(semID, MUTEXP, SETVAL, 1);
	semctl(semID, MUTEXC, SETVAL, 1);

	/*
	Se sono arrivato qui, ho correttamente settato tutto.
	Ora devo solo far girare il programma con i produttori e consumatori in funzione.
	*/

	const unsigned short int numProd = 25;
	const unsigned short int numCons = numProd;
	const unsigned short int numProc = numProd + numCons;

	for(int i = 0; i < numProd; ++i){
		pid_t pid = fork();

		switch(pid){
		case -1:
			fprintf(stderr, "FATAL ERROR! Cannot fork process\n");
			exit(5);
			break;

		case 0: ;
			type product = i;
			produttore(semID, bufferPtr, vectorPtr, product);
			printf("Produttore %d, ho prodotto %d\n", i, product);
			exit(0);
			break;

		default:
			break;
		}
	}

	for(int i = 0; i < numCons; ++i){
		pid_t pid = fork();

		switch(pid){
		case -1:
			fprintf(stderr, "FATAL ERROR! Cannot fork process\n");
			exit(5);
			break;

		case 0: ;
			type t = consumatore(semID, bufferPtr, vectorPtr);
			printf("Consumatore %d, ho letto %d\n", i, t);
			exit(0);
			break;

		default:
			break;
		}
	}

	/* Al termine dell'esecuzione dei figli, devo prima aspettare la loro terminazione
	e poi rimuovere le strutture dati per non sporcare inutilmente la memoria. */
	for(int i = 0; i < numProc; ++i){
		wait(NULL);
	}

	semctl(semID, 0, IPC_RMID);
	shmctl(shmBufferID, IPC_RMID, 0);
	shmctl(shmVectorID, IPC_RMID, 0);
}
