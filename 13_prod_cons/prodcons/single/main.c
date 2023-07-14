#include <stdio.h> // printf() scanf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork()
#include <sys/types.h> // pid_t key_t
#include <wait.h> // wait()
#include <sys/shm.h> // shmget() shmctl()
#include <sys/sem.h> // semget() semctl()

#include "semaphore.h" // waitSem() signalSem()

const int SPAZIO_DISP = 0;
const int MSG_DISP = 1;
typedef int type;

void produttore(int id_sem, type *shmPtr, type newObj){
	waitSem(id_sem, SPAZIO_DISP);

	*shmPtr = newObj;

	signalSem(id_sem, MSG_DISP);
}

type consumatore(int id_sem, type *shmPtr){
	waitSem(id_sem, MSG_DISP);

	type t = *shmPtr;

	signalSem(id_sem, SPAZIO_DISP);

	return t;
}

int main() {
	key_t key = ftok(".",'C');

	int id_shm = shmget(key, sizeof(type), IPC_CREAT|IPC_EXCL|0664);
	if(id_shm < 0) {
		// shared memory was already in memory. I'm gonna get its ID
		id_shm = shmget(key, sizeof(type), 0664);

		if(id_shm < 0){
			fprintf(stderr, "Fatal error! Cannot istantiate shared memory!\n");
			exit(1);
		}
	}

	int id_sem = semget(key, 2, IPC_CREAT|0664);

	if(id_sem < 0){
		fprintf(stderr, "Fatal error! Cannot istantiate semaphores\n");
		exit(2);
	}

	// Ho sia shared memory che semaforo
	// Ora devo settare entrambi

	type *ptr;
	ptr = (type*)shmat(id_shm,0,0);

	if(ptr == (void*)-1){
		fprintf(stderr, "Fatal error! Cannot attach shared memory to variable!\n");
		exit(3);
	}

	semctl(id_sem, SPAZIO_DISP, SETVAL, 1);
	semctl(id_sem, MSG_DISP, SETVAL, 0);

	const int numProcessi = 20;

	for(int i = 0; i < numProcessi; ++i){
		pid_t pid = fork();

		if(pid < 0){
			fprintf(stderr, "Fatal error! Cannot fork process\n");
			exit(4);
		}
		if(pid == 0){
			if( i % 2 == 0){
				produttore(id_sem, ptr, i);
			} else {
				type ret = consumatore(id_sem, ptr);
				printf("Sono il processo %d e ho letto %d\n", i, ret);
			}
			exit(0);
		}
	}

	// Aspetto la fine di tutti i figli per continuare
	for(int i = 0; i < numProcessi; ++i){
		wait(NULL);
	}

	// Rimuovo le strutture dati create nel programma
	semctl(id_sem, 0, IPC_RMID);
	shmctl(id_shm, IPC_RMID, 0);
}
