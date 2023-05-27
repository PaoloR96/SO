#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void){
	
	int pid,ppid;
	int valore;
	int status;
	
	/*
	 *  SONO IL PROCESSO PADRE
	 *  tutto quello presente nel main tranne il la parte della if( variabile == 0){ codice } fa parte del padre
	 *	tutto quello che accade prima della chiamata fork()
	 *  (modifica alle variabili, etc..) , verrà fatta una copia esatta anche al figlio
	 *
	 */
	
	// assegno un pid al padre
	pid = getpid();
	
	// mi faccio tornare il pid del processo che ha generato il main
	ppid = getppid();
	
	//serie di istruzioni che svolge il padre
	printf("\n\n");
	printf("Sono il padre del processo ho pid : %d",pid);
	printf("\n");
	printf("il processo che mi ha generato ha pid : %d",ppid);
	printf("\n");
	
	/* chiamata a fork() generazione del figlio */
	valore = fork();
	
	// da qunando entra in questo if e tutto codice del figlio ed è un nuovo processo indipendente dal padre
	if(valore == 0){
		
		/*
		 *  SONO IL PROCESSO FIGLIO
		 *  tutto quello presente in qiesto if non andra ad influenzare in nessuna maniera le variabili del padre
		 *	uso le stesse variabili del padre ma lavoro su una loro copia (facendo la fork() faccio una copia del proceso padre [u-area heap-area ...]),
		 *  ma faccio un lavoro apparte qui posso scrivere un altro codice o usare exec() per caricare un nuovo codice etc..
		 *	questo non influenzera il padre
		 *
		 */
		
		// serie di itruzioni che svolge il figlio
		printf("\n\nSono il figlio e ho pid : %d",pid = getpid());
		printf("\n");
		printf("il processo che mi ha generato ha pid : %d",ppid = getppid());
		
		// addormento un po il processo figlio  
		sleep(3);
		
		// serve a terminare il figlio e restituisce un valore al padre che è in attesa con wait() della sua terminazione
		exit(0);
	}
	
	if(valore == -1){
	
		/*IL FIGLIO NON E STATO GENERATO A CAUSA DI UN QUALCHE ERRORE*/
		
		// scrivo cofa deve fare se succede questo
		printf("\nErrore non ho potuto creare il figlio\n");
	
	}
	
	/*SONO ANCORA NEL PROCESSO PADRE*/
		
	// addormento un po il processo padre 
	sleep(7);
	
	/* 
	 * la funzione wait() ci permette di aspettare che il figlio termini e ci restituisce un valore preso dalla exit() del figlio
	 * se questo valore è =0 allora il figlio ha terminato correttamente se restituisce un valore != 0 allora il figlio
	 * e terminato involontariamente.
	 *
	 * facendo wait(&status); dove status lo abbiamo dichiarato intero inizialmente, dico alla wait di salvare la risposta
	 * data dal figlio nell'indirizzo di status con wait(&status) e poi dopo vado a verificare status mediante il suo contenuto
	 *
	 */
	valore = wait(&status);
	
	// vado a verificare lo stato di terminazione del figlio
	if(WIFEXITED(status)){	
		// WIFEXITED(status)resttuisce vero se il figlio ha terminato volontariamente
		printf("\nTerminazione volontaria del processo figlio con pid :  %d con stato %d\n",pid,WEXITSTATUS(status));
	}
	
	if(WIFSIGNALED(status)){
		// WIFSIGNALED(status)restituisce vero se il figlio ha terminato invlontariamente
		printf("\nTerminazione involontaria del processo figlio con pid : %d per segnale %d\n",pid,WTERMSIG(status));
		// WTERMSIG(status) contiene il numero dell'interruzione che ha causato la terminazione
	}
	
	// posso terminare il padre
	printf("\nTerminazione del processo padre pid : %d",pid);
	printf("\n\n");
	
	return 0;
}
