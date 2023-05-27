#include "procedure.h"

/*Il processo figlio mittente eseguirà un loop in cui ad ogni iterazione si mette in attesa di una
stringa dall'utente dallo standard input, ed invia un messaggio con la stringa sulla prima coda di
messaggi condivisa. Quando l'utente inserisce "exit" seguito da un carattere di invio, il programma
deve inviare un messaggio con una stringa "exit" sulla seconda coda e terminare.
*/

void mittente(){
	
	int ret;
	msg m;

	
	while(1){
		m.type=TIPO;		
				
		printf("MESSAGGIO DA INVIARE:\n");
		scanf("%s",m.mess);
	

		if(strcmp(m.mess,"exit")!=0){
		
			
		ret=msgsnd(coda_a,&m,sizeof(msg)-sizeof(long),0);
		if(ret<0){
			 perror("CODA_A errore invio");
			_exit(0);
			}
	
		
		}else{

		
		ret=msgsnd(coda_b,&m,sizeof(msg)-sizeof(long),0);
		if(ret<0){
			 perror("CODA_B errore invio");
			_exit(0);
			}
		_exit(0);
		}
		
	
	}
	
	


}



/*Il processo figlio ricevente eseguirà un loop in cui ad ogni iterazione si metterà in attesa di un
messaggio dalla seconda coda, e stamperà sullo standard output la stringa ricevuta. In caso di
ricezione di un messaggio con la stringa "exit", il processo dovrà terminare.*/

void destinatario(){
		
	int ret;
	msg m;
	
	while(1){
	
		ret=msgrcv(coda_b,&m,sizeof(msg)-sizeof(long),TIPO,0);
		if(ret<0) {
			perror("CODA_B errore ricezione");
			_exit(0);
		}
		

		
		printf("messaggio ricevuto: %s\n",m.mess);
		

		if(strcmp(m.mess,"exit")==0)	
		_exit(0);			

	}



}

