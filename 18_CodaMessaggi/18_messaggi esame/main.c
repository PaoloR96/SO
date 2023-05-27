#include "lib.h"

int queue_ric,queue_risp;

int main(){
	
	pid_t pid;
	int i,st;
	messaggio m;
	key_t k_s=ftok(".",'b');
	
	int queue_ric=msgget(k_s,IPC_CREAT|0664);
	
	for(i=0;i<4;i++){
		pid=fork();
		if(pid==0){
			if(i<3)
				execl("./client","./client",NULL);
			else
				execl("./server","./server",NULL);
			_exit(0);
		}
	}
	
	//ATTENDO TERMINAZIONE CLIENT
	for(i=0;i<3;i++){
		pid=wait(&st);
		printf("MAIN Client %d terminato con stato: %d\n",pid,st>>8);
	}
	
	sleep(3);
	
	//INVIO MESSAGGIO TERMINAZIONE SERVER
	m.tipo=RICHIESTA;
	m.a=-1;
	m.b=-1;
	msgsnd(queue_ric,&m,DIM,0);
	printf("CLIENT Inviata send terminazione\n");
	//ATTENDO TERMINAZIONE SERVER
	pid=wait(&st);
	printf("MAIN Server %d terminato con stato: %d\n",pid,st>>8);
	
	msgctl(queue_ric,0,IPC_RMID);
	msgctl(queue_risp,0,IPC_RMID);
	
	return 0;
}