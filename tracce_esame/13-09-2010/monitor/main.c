#include <unistd.h> // fork
#include <sys/types.h> // pid_t
#include <stdlib.h> // exit, NULL
#include <sys/wait.h> // wait
#include <sys/shm.h> // shmget, shmctl
#include <sys/ipc.h> // IPC macros

#include "header.h"

int main(){

	int magazz_shm = shmget( IPC_PRIVATE, sizeof(Magazzino), IPC_CREAT|IPC_EXCL|0664);
	int monitor_shm = shmget( IPC_PRIVATE, sizeof(Monitor), IPC_CREAT|IPC_EXCL|0664);

	Magazzino *magazzino = (Magazzino*)shmat(magazz_shm, 0, 0);
	Monitor *monitor = (Monitor*)shmat(monitor_shm, 0, 0);

	init_magazzino(magazzino);

	init_monitor(monitor, 2); // due variabili condition

	pid_t pid;

	for(int i = 0; i < NUM_FORNITORI; ++i){
		pid = fork();
		if(pid == 0){
			fornitore(magazzino, monitor);
			exit(0);
		}
	}

	for(int i = 0; i < NUM_CLIENTI; ++i){
		pid = fork();
		if(pid == 0){
			cliente(magazzino, monitor);
			exit(0);
		}
	}

	for(int i = 0; i < NUM_PROC; ++i)
		wait(NULL);

	remove_monitor(monitor);
	shmctl( magazz_shm, IPC_RMID, 0);
	shmctl( monitor_shm, IPC_RMID, 0);
}
