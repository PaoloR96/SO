#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

//libreire da includere per la shared memory
#include <sys/shm.h>
#include <sys/ipc.h>

/*
 * shmid_ds è definito nell'header <sys/shm.h>
 *
 * struct shmid_ds{
 * 		struct ipc_perm shm_perm;			permessi
 *		size_t shm segsz;					dim del segmento in bytes
 * 		__time_t shm_atime;					tempo ultimo shmat()
 * 		__time_t shm_dtime;					tempo ultimo shmdt()
 *		__time_t shm_ctime;					tempo ultimo shmctl()
 *		__pid_t shm_cpid;					pid del creatore
 *		__pid_t shm_lpid;					pid dell'ultimo operatore
 *		shmatt_t shm_nattch;				numero di attach. correnti
 * };
 *
 * int shmget(key_t key, int size, int flag);
 * void* shmat(int shmid, const void * shmaddr, int flag);
 * int shmctl(int ds_shm, int cmd, struct shmid_ds * buff);
 *
 *
 */
int main(){
	
	//creo una variabile che conterra il valore della mia chiave 
	int mykey;
	int pid;
	int ds_shm;  //sara il descrittore della nostra shm (shared memory)
	int valore;
	int status;
	int ppid;
	
	//puntatore ad un char
	char *p;
	char iniz[] = " Questa è una prova ";
	
	/*PADRE*/
	
	pid = getpid();
	ppid = getppid();
	printf("\n\nSono il padre con pid : %d\n",pid);
	
	
	/*
	 * assegno il valore alla chiave tramite ftok();
	 *
	 * IPC KEYS : ftok(char* path, char id) , per far si che due programmi generino la stessa chiave
	 * indiachiamo lo stesso path e la stessa id generamente indicata con una lettera vedi esempio sotto
	 * il "." indica il path corrente.
	 *
	 * NB ogni processo ha un proprio desrittore deove sono memorizzati i seguenti dati:
	 *    	nome del processo: generalmente un indice
	 *		stao del processo   
	 *		modalita di servizio 
	 *		informazioni sulla gestione della memoria
	 *		contesto del processo
	 *		utilizzo risorse
	 *		identificatore del processo successivo
	 */
	
	mykey = ftok(".",'u');
	printf("\n\nla chiave e : %d \n\n",mykey);
	
	/* int shmget(key_t key, int size, int flag ); creazione di un segmento di memoria condivisa
	 *	key:  chiave per identificare la shm in maniera univoca nel sistema si puo ottenere tramite ftok();
	 *	size: dimensione in byte della memoria condivisa
	 *	flag: (IPC_CREAT,IPC_EXCL,permessi) specifica modalità di creazione epermessi di accesso
	 *		  per i permessi controllare la lezione sui permessi (in esadecimale)
	 *
	 * ds_shm e la variabile in cui mi vado a salvare il risultato della shmget(); 
	 *
	 * per come scritta se gia esiste la memoria condivisa non va a crearne una nuova ma riutilizza quella esistente
	 * se al posto della key inserisco IPC_PRIVATE ne creo una locale utilizzabile solo dal padre e figli
	 * ds_shm = shmget(IPC_PRIVATE, 1024, IPC_CREAT | IPC_EXCL | 0664);
	 *
	 */
	//assegno il descrittore della shared mamemory
	ds_shm = shmget(mykey,1024,IPC_CREAT | IPC_EXCL | 0664); 
	printf("il descrittrore della risorsa codivisa è : %d",ds_shm);
	
	//verifico se ha creato la risorsa codivisa
	if(ds_shm >= 0){
		
		//tutto quello che scrivo in qiesto if e la creazione ed inizializzazione della mia memoria condiviasa(shared memory)
		//vedere piu giu la dichiarazione di void* shmat();
		/*
			
		
		
		*/
		p = (char*) shmat(ds_shm,NULL,0);
		
		//ho inizializzato la shm (shared memory), in questo caso contiene solo un vettore di char
		strncpy(p,iniz,sizeof(iniz));
	
	}else{
		//la risorsa già esiste e me la vado a richiamare per usarla inutile crearne una nuova, da notare che non ho messo i flag
		ds_shm = shmget(mykey, 1024, 0664);
		//qui non ho bisogno di inizializzare perche la risorsa già esiste
		p = (char*) shmat(ds_shm,NULL,0);
	}
	
	//CONTROLLO IL CONTENUTO DELLA SHM
	printf("\n\nSONO IL PADRE CONTENUTO SHM  : %s \n\n", p);
	
	
	//genero un figlio che usera la shared memory
	valore = vfork();
	
	if(valore == 0){
		/*FIGLIO*/
		
		pid = getpid();
		ppid = getppid();
		printf("\n\nSono il figlio con pid : %d\n",pid);
		
		//qui mi vado a richiamare la memoria(risorsa) condivisa tramite ds_shm(il suo descrittore)
		/*
		 *  shared memory attach collega il segmento di memoria allo spazio di indirizzamento del chiamante
		 *	
		 *  void* shmat(int shmid, const void* shmaddr, int flag);
		 *		shmid: identificatore del segmento di memoria
		 *		shmaddr: indirizzo dell'area di memoria del processo chiamante al quale 
		 *				 collegare il segmento di memoria condivisa
		 *				 se 0 un valore opportuno viene automaticamnete scelto
		 *		flag: opzioni(IPC_RDONLY solo lettura)
		 *		
		 *	restituisce l'indirizzo del segmento collegato, e -1 se fallisce
		 *
		 *
		 *
		 */
		p = (char*) shmat(ds_shm,NULL,0);
		printf("\n\nSONO IL FIGLIO CONTENUTO SHM  : %s \n\n", p);
		
		int i;
		
		for(i =0; i<sizeof(iniz);i++){
			iniz[i] = 'c';
		}
		
		strncpy(p,iniz,sizeof(iniz));
		printf("\n\nSONO IL FIGLIO CONTENUTO SHM  : %s \n\n", p);
		
		sleep(4);
		
		printf("\nsono il figlio sono terminato\n\n");
		exit(0);
	}
	if(valore == -1){
		printf("\n\nfiglio non creato\n\n");
		exit(1);
	}
	
	printf("\n\nSONO IL PADRE CONTENUTO SHM  : %s \n\n", p);

	//rimuove la shared memory creata
	/*
	 * int shmctl(int ds_shm, int cmd, struct shmid_ds * buff)
	 *		
	 * 		Invoca l’esecuzione di un comando su una SHM
  	 *		ds_shm: descrittore della memoria condivisa su cui si vuole operare
  	 *		cmd: specifica del comando da eseguire:
  	 *		IPC_STAT, IPC_SET, IPC_RMID: marca da eliminare, rimuove solo quando non vi sono più processi attaccati
 	 *		SHM_LOCK: impedisce che il segmento venga swappato o paginato
  	 *		buff: puntatore ad una struttura di tipo shmid_ds con eventuali
	 *		parametri per i comandi IPC_STAT e IPC_SET
	 *
	 * restituisce -1 in caso di fallimento
	 * 
	 */
	shmctl(ds_shm, IPC_RMID, 0);
	
	//controllo sulla terminazione del processo figlio
	valore = wait(&status);
	
	if(WIFEXITED(status)){	
		// WIFEXITED(status)resttuisce vero se il figlio ha terminato volontariamente
		printf("\nTerminazione volontaria del processo figlio con pid :  %d con stato %d\n",pid,WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status)){
		// WIFSIGNALED(status)restituisce vero se il figlio ha terminato invlontariamente
		printf("\nTerminazione involontaria del processo figlio con pid : %d per segnale %d\n",pid,WTERMSIG(status));
		// WTERMSIG(status) contiene il numero dell'interruzione che ha causato la terminazione
	}
	
	
	
	printf("sono il padre sono terminato\n\n");
	
	return 0;
}
