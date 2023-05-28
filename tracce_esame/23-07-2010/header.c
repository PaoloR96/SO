#include "header.h"
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void client(int msg_queue){
	pid_t message = getpid(); // crea il messaggio
	
	MessageClientServer mcs;
	mcs.type = SERVER_QUEUE; // manda al server
	mcs.message = message; // manda il messaggio (pid)
	
	for(int i = 0; i < CNT_CLIENT; ++i){
	
		if (msgsnd(msg_queue, &mcs, sizeof(mcs) - sizeof(mcs.type), 0) < 0 ){
			printf("Fatal error with msgsnd / PID = %d\n", getpid() );
			exit(1);
		}
				
		sleep(1);
	} // for(;;)
} // client()

void server(int msg_queue){
	MessageClientServer buffer[SIZE];
	MessageClientServer temp;
	int index = 0;
	
	while(1){
		if (msgrcv(msg_queue, &temp, sizeof(temp) - sizeof(temp.type), SERVER_QUEUE, 0) < 0 ){
			printf("Fatal error with msgrcv from client (SERVER)\n");
			exit(1);
		}
		
		buffer[index] = temp;
		index = (index + 1) % SIZE;
		
		if (index == 0) { // sono tornato in testa == buffer pieno
			MessageServerPrinter m;
			m.type = PRINTER_QUEUE; // manda alla stampante
			
			for(int i = 0; i < SIZE; ++i)
				m.array[i] = buffer[i]; // popola l'array da stampare
			
			if (msgsnd(msg_queue, &m, sizeof(m) - sizeof(m.type), 0) < 0 ){
				printf("Fatal error with msgsnd to printer (SERVER)\n");
				exit(1);
			}
		} // if index == 0
	} // while(1)
} // server()

void printer(int msg_queue){
	MessageServerPrinter m;
	
	while(1){
		if (msgrcv(msg_queue, &m, sizeof(m) - sizeof(m.type), PRINTER_QUEUE, 0) < 0 ){
			printf("Fatal error with msgrcv from server (PRINTER)\n");
			exit(1);
		}
		
		for(int i = 0; i < SIZE; ++i)
			printf("Msg[%d] = %d\n", i, m.array[i].message);
			
		printf("\n");
		
	} // while(1)
} // printer()
