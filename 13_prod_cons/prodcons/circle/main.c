#include <stdio.h> // printf, scanf
#include <stdlib.h> // exit, NULL
#include <unistd.h> // fork
#include <sys/types.h> // pid_t, key_t
#include <sys/sem.h> // semget, semctl
#include <sys/shm.h> // shmget, shmctl
#include <sys/ipc.h> // IPC macro
#include <wait.h> // wait

#include "semaphore.h" // waitSem, signalSem

//// SHARED MEMORY
// SIZE is the size of our buffer of elements
const unsigned short int SIZE = 20;
// We use a typedef to be more general as possible
typedef int type;
//------------------------------

//// SEMAPHORE
const unsigned short int SPAZIO_DISP = 0, MSG_DISP = 1, MUTEXP = 2, MUTEXC = 3;

//------------------------------
//// FUNCTIONS
void prod(int sem_id, type *bufferPtr, int *headPtr, type product){
	waitSem(sem_id, SPAZIO_DISP);
	waitSem(sem_id, MUTEXP);
	// sezione protetta
	// se entro qui sono sicuro che c'è spazio disponibile e che nessun altro produttore è presente

	*(bufferPtr + *headPtr) = product;
	printf("Ho scritto %d\n", product);

	*headPtr += 1;
	if(*headPtr == SIZE){
		*headPtr = 0;
	}

	// fine sezione protetta
	signalSem(sem_id, MUTEXP);
	signalSem(sem_id, MSG_DISP);
}

type cons(int sem_id, type *bufferPtr, int *tailPtr){
	waitSem(sem_id, MSG_DISP);
	waitSem(sem_id, MUTEXC);
	// sezione protetta
	// se entro qui sono sicuro che c'è un messaggio e che nessun altro consumatore è presente

	type temp = *(bufferPtr + *tailPtr);
	printf("Ho letto %d\n", temp);

	*tailPtr += 1;
	if(*tailPtr == SIZE){
		*tailPtr = 0;
	}

	// fine sezione protetta
	signalSem(sem_id, MUTEXC);
	signalSem(sem_id, SPAZIO_DISP);

	return temp;
}

//------------------------------
//// MAIN

int main(){
	// Initially I got keys with ftok function
	key_t shmBufferKey = ftok(".", 'C');
	key_t shmHeadKey = ftok(".", 'H');
	key_t shmTailKey = ftok(".", 'T');
	key_t semKey = ftok(".", 'S');

	// Then I get ids from shmget...
	int shmBufferID = shmget(shmBufferKey, sizeof(type) * SIZE, IPC_CREAT|IPC_EXCL|0664);
	if(shmBufferID < 0){
		// Resource already exist in memory, then we have to just attach to it
		shmBufferID = shmget(shmBufferKey, sizeof(type) * SIZE, 0664);
		if(shmBufferID < 0){
			fprintf(stderr, "FATAL ERORR! Cannot connect to shared memory!\n");
			exit(1);
		}
	}

	int shmHeadID = shmget(shmHeadKey, sizeof(int), IPC_CREAT|IPC_EXCL|0664);
	if(shmHeadID < 0){
		// Resource already exist in memory, then we have to just attach to it
		shmHeadID = shmget(shmHeadKey, sizeof(int), 0664);
		if(shmHeadID < 0){
			fprintf(stderr, "FATAL ERORR! Cannot connect to shared memory!\n");
			exit(1);
		}
	}

	int shmTailID = shmget(shmTailKey, sizeof(int), IPC_CREAT|IPC_EXCL|0664);
	if(shmTailID < 0){
		// Resource already exist in memory, then we have to just attach to it
		shmTailID = shmget(shmTailKey, sizeof(int), 0664);
		if(shmTailID < 0){
			fprintf(stderr, "FATAL ERORR! Cannot connect to shared memory!\n");
			exit(1);
		}
	}

	// ... and from semget
	int semID = semget(semKey, 4, IPC_CREAT|0664);
	if(semID < 0){
		fprintf(stderr, "FATAL ERROR! Cannot create semaphore!\n");
		exit(2);
	}

	// Now we have to attach the shmIDs with shmat (1), and setvalue for sem (2)

	// (1)
	type *bufferPtr;
	bufferPtr = (type*)shmat(shmBufferID, 0, 0);
	if( bufferPtr == (void*)-1 ){
		fprintf(stderr, "FATAL ERROR! Cannot attach buffer shared memory to variable!\n");
		exit(3);
	}

	int *headPtr, *tailPtr;
	headPtr = (int*)shmat(shmHeadID, 0, 0);
	if( headPtr == (void*)-1 ){
		fprintf(stderr, "FATAL ERROR! Cannot attach shared memory to head ptr!\n");
		exit(4);
	}

	tailPtr = (int*)shmat(shmTailID, 0, 0);
	if( tailPtr == (void*)-1 ){
		fprintf(stderr, "FATAL ERROR! Cannot attach shared memory to tail ptr!\n");
		exit(5);
	}

	// (2)
	semctl(semID, SPAZIO_DISP, SETVAL, SIZE);
	semctl(semID, MSG_DISP, SETVAL, 0);
	semctl(semID, MUTEXP, SETVAL, 1);
	semctl(semID, MUTEXC, SETVAL, 1);

	// Now we have to test the program!

	const short unsigned int numCons = 20;
	const short unsigned int numProd = numCons;
	const short unsigned int numProc = numCons + numProd;

	for(int i = 0; i < numCons; ++i){
		pid_t pid = fork();

		if(pid < 0){
			fprintf(stderr, "FATAL ERROR! Cannot fork process!\n");
			exit(6);
		}
		if(pid == 0){
			cons(semID, bufferPtr, tailPtr);

			exit(0);
		}
	}

	for(int i = 0; i < numProd; ++i){
		pid_t pid = fork();

		if(pid < 0){
			fprintf(stderr, "FATAL ERROR! Cannot fork process!\n");
			exit(6);
		}
		if(pid == 0){
			prod(semID, bufferPtr, headPtr, i);

			exit(0);
		}
	}

	for(int i = 0; i < numProc; ++i){
		wait(NULL);
	}

	semctl(semID, 0, IPC_RMID);
	shmctl(shmBufferID, IPC_RMID, 0);
	shmctl(shmHeadID, IPC_RMID, 0);
	shmctl(shmTailID, IPC_RMID, 0);
}
