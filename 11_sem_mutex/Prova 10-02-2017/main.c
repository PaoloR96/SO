#include "header.h"

int main (){
	
	int status;
	pid_t pid;
	
	srand(time(NULL));
	
	int id_sem = semget (IPC_PRIVATE, 3, IPC_CREAT | 0664);
	
	semctl (id_sem, PROD, SETVAL, 0);
	semctl (id_sem, CONS, SETVAL, 0);
	semctl (id_sem, MUTEX, SETVAL, 1);
	
	
	int shm_b = shmget (IPC_PRIVATE, sizeof (Buffer), IPC_CREAT | 0664);
	
	Buffer *b = (Buffer *) (shmat (shm_b, 0, 0));
	
	int *v;
	
	int shm_v = shmget (IPC_PRIVATE, sizeof(int [10000]), IPC_CREAT | 0664);
	
	v = (int *) (shmat (shm_v, 0, 0));
	
	for (int i=0; i<10000; i++){
	
		v[i] = rand ()%100000+1;
		
		printf ("ELEMENTO [%d] - %d\n", i+1, v[i]);
		
	} 
	
	printf ("\n\n");
	
	for (int i=0; i<10; i++){
		
		pid = fork ();
		
		if (pid == 0){
		
			printf ("\nSono il Figlio <%d>. INDICE: %d\n", getpid(), i);
			Produzione_Minimo (b, v, id_sem, i);
			
			exit (0);
		
		}
	}
	
	
	Consumo_Minimo (b, id_sem);
	
	
	for (int i=0; i<10; i++){
	
		pid = wait(&status);
		
		if (pid == -1){
		
			printf ("Errore\n");
		
		}else{
		
			printf ("Il Figlio <%d> e' terminato con status %d\n", pid, status);
		
		}
	
	}
	
	shmctl (shm_v, 0, IPC_RMID);
	printf ("Shared Memory - Vettore: RIMOSSA!\n");
	
	shmctl (shm_b, 0, IPC_RMID);
	printf ("Shared Memory - Buffer: RIMOSSA!\n");
	
	semctl (id_sem, IPC_RMID, 0);
	printf ("Semafori rimossi!\n");
	
	return 0;

}
