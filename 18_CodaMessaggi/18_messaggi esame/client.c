#include "lib.h"

int queue_ric,queue_risp;

int main(){
	
	int i;
	messaggio m_ric,m_risp;
	key_t k_ric=ftok(".",'b');
	key_t k_risp=ftok(".",'c');
	
	queue_ric=msgget(k_ric,IPC_CREAT|0664);
	queue_risp=msgget(k_risp,IPC_CREAT|0664);
			   
	srand(time(NULL)^getpid());
	
	for(i=0;i<5;i++){
		
		//INVIO RICHIESTA
		m_ric.pid=getpid();
		m_ric.a=rand()%11;
		m_ric.b=rand()%11;
		m_ric.tipo=RICHIESTA;
		msgsnd(queue_ric,&m_ric,DIM,0);
		printf("CLIENT %d Inviato a: %d b: %d\n",m_ric.pid,m_ric.a,m_ric.b);
		
		//ATTENDO RISPOSTA
		msgrcv(queue_risp,&m_risp,DIM,getpid(),0);
		printf("CLIENTE %d Risposta ricevuta Prodotto: %d\n",getpid(),m_risp.prodotto);
	}
	
	//msgctl(queue_ric,0,IPC_RMID);
//	msgctl(queue_risp,0,IPC_RMID);	
	
	return 0;
}