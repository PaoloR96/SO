#include "header.h"

int main (){
	
	pid_t pid;
	int i, status;
	int id_ric, id_ris;
	key_t k_ric, k_ris;
	
	k_ric = IPC_PRIVATE;
	k_ris = IPC_PRIVATE;
	
	id_ric = msgget (k_ric, IPC_CREAT | 0664);
	id_ris = msgget (k_ris, IPC_CREAT | 0664);
	
	srand (time(NULL));
	
	Magazzino g;
	
	InizializzaMagazzino (&g);
	
	for (i=0; i<3; i++){
		pid = fork ();
	
		if (pid == 0){
		
			printf ("Sono il figlio Client <%d>\n", getpid());
			Client (id_ric, id_ris);
		
			exit (0);
	
		}
	}
	
	for (i=0; i<2; i++){
	
		pid = fork ();
		
		if (pid == 0){
			
			printf("Sono il figlio Fornitore <%d>\n",getpid());
			
			Fornitore (id_ric, id_ris);
			
			exit (0);
		
		
		}
	
	
	}
	
	pid = fork ();
	
	if (pid == 0){
		
		printf ("Sono il figlio Gestore <%d>\n", getpid());
		Gestore (&g, id_ric, id_ris);
		
		exit (0);
	
	}
	
	for (i=0; i<6; i++){
		
		pid = wait (&status);
		
		if (pid == -1){
			
			printf ("Errore\n");
		}else{
			
			printf ("Il processo <%d> e' terminato con stato %d\n", pid, status);
		
		}
	
	}
	
	msgctl (id_ric, IPC_RMID, 0);
	msgctl (id_ris, IPC_RMID, 0);
	printf ("Code di messaggi rimosse correttamente!\n");
	
	return 0;
}
