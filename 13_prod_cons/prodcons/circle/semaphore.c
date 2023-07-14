#include "semaphore.h" // waitSem, signalSem implementation

#include <sys/sem.h> // struct sembuf, semop

void waitSem(int sem_id, int sem_num){
	struct sembuf sem_buf;

	sem_buf.sem_num = sem_num;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = -1;

	semop(sem_id, &sem_buf, 1);
}

void signalSem(int sem_id, int sem_num){
	struct sembuf sem_buf;

	sem_buf.sem_num = sem_num;
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = +1;

	semop(sem_id, &sem_buf, 1);
}
