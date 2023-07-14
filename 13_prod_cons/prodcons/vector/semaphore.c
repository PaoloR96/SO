#include <sys/sem.h> // struct sembuf, semop
#include "semaphore.h" // waitSem, signalSem

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
