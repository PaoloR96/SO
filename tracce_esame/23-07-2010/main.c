#include <stdio.h> // printf
#include <stdlib.h> // exit, NULL
#include <unistd.h> // fork
#include <sys/wait.h> // wait
#include <sys/types.h> // pid_t
#include <sys/msg.h> // msgget, msgctl
#include <sys/ipc.h> // IPC macros

#include <signal.h> // kill process

#include "header.h"

int main(int argc, char *argv[]){
	const short num_proc = 5 + 1 + 1; // 5 client, 1 server, 1 printer
	const short time_to_wait = 15; // secondi da aspettare per terminare i figli

	pid_t pid; // appoggio
	pid_t children[num_proc]; // vettore dei figli da terminare
	short index = 0; // indice del vettore
	
	int msg_queue_id = msgget( IPC_PRIVATE , IPC_CREAT | IPC_EXCL | 0664 );
	
	if (msg_queue_id < 0){
		msg_queue_id = msgget( IPC_PRIVATE , 0664 );
		
		if (msg_queue_id < 0){
			printf("Cannot attach msg_queue to var\n");
			exit(1);
		}
	}
	
	pid = fork();
	if (pid == 0){
		server(msg_queue_id);
		exit(0);
	}
	children[index] = pid;
	++index;
	
	pid = fork();
	if (pid == 0){
		printer(msg_queue_id);
		exit(0);
	}
	children[index] = pid;
	++index;
	
	for(int i = 0; i < 5; ++i){
		pid = fork();
		if (pid == 0){
				client(msg_queue_id);
				exit(0);
		}
		children[index] = pid;
		++index;
	}
	
	sleep(time_to_wait);
	
	for(int i = 0; i < num_proc; ++i)
		kill(children[i], SIGTERM);
		
	msgctl(msg_queue_id, IPC_RMID, 0);
	
	printf("Fine esecuzione. Arrivederci\n");
}
