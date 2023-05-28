#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>

#include "header.h"

int main(){
	key_t key = ftok(PATH_Q, KEY_Q);
	pid_t pid;
	
	int server_qid = msgget(key, IPC_CREAT|IPC_EXCL|0664);
	if( server_qid < 0 ){
		server_qid = msgget(key, 0664);
		
		msgctl(server_qid, IPC_RMID, 0);
		
		server_qid = msgget(key, IPC_CREAT|0664);
		
		if( server_qid < 0 ){
			fprintf(stderr, "CANNOT GET MSG QUEUE\n");
			exit(1);
		}
	}
	
	for( int i = 0; i < NUM_PROC; ++i){
		if( i < NUM_PROC_SERVER ){
			pid = fork();
			if( pid == 0 ){
				execl("./server", "./server", NULL );
				exit(0);
			}
		} else {
			pid = fork();
			if( pid == 0 ){
				execl("./client", "./client", NULL );
				exit(0);
			}
		}
	}
	
	for(int i = 0; i < NUM_PROC_CLIENT; ++i)
		wait(NULL);
			
	// quando sono qui, hanno terminato tutti i client
	// faccio terminare il server
	MessageCS m;
	m.n1 = -1;
	m.n2 = -1;
	
	msgsnd( server_qid, &m, sizeof(m) - sizeof(m.type), 0 );
	
	for(int i = 0; i < NUM_PROC_SERVER; ++i)
		wait(NULL);
	printf("MAIN: Il server ha terminato correttamente\n");
	
	// rimuovo la coda
	msgctl( server_qid, IPC_RMID, 0 );
}
