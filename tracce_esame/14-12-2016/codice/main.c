#include "header.h"
#include "../monitor/monitor.h"
#include <stdio.h> // printf
#include <stdlib.h> // exit
#include <unistd.h> // fork
#include <sys/shm.h> // shmget, shmctl
#include <sys/ipc.h> // IPC macros
#include <sys/types.h> // pid_t
#include <sys/wait.h> // wait

int main() {
	int num_proc = NUM_CLIENT_POST + NUM_CLIENT_FIN + 1;
	
	int shm_id = shmget( IPC_PRIVATE, sizeof(CSMonitor), IPC_CREAT | IPC_EXCL | 0664 );
	if( shm_id < 0 ){
		shm_id = shmget( IPC_PRIVATE, sizeof(CSMonitor), 0664 );
		if( shm_id < 0 ){
			printf( "Cannot create shm! Quitting...\n");
			exit(1);
		}
	}
	
	CSMonitor *myMonitor = (CSMonitor*)shmat(shm_id, 0, 0);
	
	if( myMonitor == (void*)-1 ){
		printf( "Cannot attach shm! Quitting...\n");
		exit(1);
	}
	
	csmonitor_init( myMonitor );
	
	for( int i = 0; i < num_proc; ++i ){
		pid_t pid = fork();
		
		if( pid == 0 ){
			if( i < NUM_CLIENT_POST )
				cliente_post( myMonitor );
			else if( i < NUM_CLIENT_POST + NUM_CLIENT_FIN )
				cliente_fin( myMonitor );
			else
				sportellista( myMonitor );
				
			exit(0);
		}
	}
	
	for(int i = 0; i < num_proc; ++i)
		wait( NULL );
		
	csmonitor_destroy( myMonitor );
	
	shmctl( shm_id, IPC_RMID, 0 );
}
