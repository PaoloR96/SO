#include <stdio.h> // printf
#include <stdlib.h> // exit, NULL
#include <wait.h> // wait
#include <unistd.h> // fork, ftok
#include <sys/types.h> // pid_t
#include <sys/ipc.h> // IPC macros
#include <sys/msg.h> // msgget, msgctl, msgsnd, msgrcv

#include "header.h"

int main(int argc, char *argv[]){
	key_t msgKey = ftok(PATH_MSG, CHAR_MSG);
	
	int msgID = msgget(msgKey, IPC_EXCL|IPC_CREAT|0664);
	if(msgID < 0){
		msgID = msgget(msgKey, 0);
		msgctl(msgID, IPC_RMID, 0); // rimuoviamo la vecchia struttura
		msgID = msgget(msgKey, IPC_CREAT|0664);
		if(msgID < 0){
			fprintf(stderr, "Fatal error! Cannot instantiate message queue in memory! Exiting...\n");
			return 1;
		}
	}
	
	pid_t pid = fork();
	if(pid == 0){
		execl("./generatore", "./generatore", NULL);
		return 0;
	}
	
	pid = fork();
	if(pid == 0){
		execl("./mediatore", "./mediatore", NULL);
		return 0;
	}
	
	for(int i = 0; i < 2; ++i)
		wait(NULL);
	
	// puliamo le risorse
	msgctl(msgID, IPC_RMID, 0);
}
