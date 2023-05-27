/*
nella shem ci sono un buffer di dim 2 (circolare) ed uno di dim 1 (una singola cella)

4 produttori, 2 produzioni - prima, sleep(5) - dopo, sleep(2 + rand()%4)
2 consumatori, 4 consumi
4 lettori che leggono dalla shmem
*/

/*SEMAFORI:

1. MUTEXP = mutua esclusione dei produttori;
2. MUTEXC = mutua esclusione dei consumatori;
3. spazio_disponibile = per il buffer B1, per i produttori;
4. messaggio_disponibile = per il buffer B1, per i consumatori;
5. cella_disponibile = è un MUTEX su B2;

Operazioni:

PRODUTTORI

1. Wait su spazio_disponibile (2-1 = 1) da parte del primo produttore
2. Wait sul MUTEXP (1-1 = 0) da parte del primo produttore
3. Produzione
4. Signal su MUTEXP (0+1 = 1)
5. Signal su messaggio_disponibile (0+1 = 1)
6. Wait su spazio_disponibile (1-1 = 0) da parte del secondo produttore
7. Wait su MUTEXP (1-1 = 0)
8. Produzione
9. Signal su MUTEXP (0+1 = 1)
10. Signal su messaggio_disponibile (1+1 = 2)
11. spazio_disponibile è a 0 e si ferma la produzione

CONSUMATORI

1. Wait su messaggio_disponibile (2-1 = 1) da parte del primo consumatore
2. Wait su MUTEXC (1-1 = 0) da parte del primo consumatore
3. Consumo (Scrive nel buffer B2)
4. Signal su cella_dispnibile
5. Lettura (L1, L2, L3, L4), assicurata dall sleep(10) dei lettori
6. Signal su MUTEXC (0+1 = 1) dai lettori
7. Signal su spazio_disponibile (0+1 = 1)
8. Wait su messaggio_disponibile (1-1 = 0) da parte del secondo consumatore
9. Wait su MUTEXC (1-1 = 0) da parte del secondo consumatore
10. Consumo (Scrive nel buffer B2)
11. Lettura
12. Signal su MUTEXC (0+1 = 1)
13. Signal su spazio_disponibile (1+1 = 2)

*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"

int main(){

//Variabili utili
int semaphore, st, i, num_processi, id_buffer;
int *ptr_cella, *testa, *coda, *ptr_buffer;
pid_t pid;
BufferCircolare *B1;

//Chiavi per i buffer	
key_t chiave_buffer_circolare = ftok(".", 'a');		//La IPC_PRIVATE non funziona because of reasons

//Chiave per i semafori
key_t chiave_sem = ftok(".", 'b');		//Forse la IPC_PRIVATE funziona, ma per sicurezza boh

//Inizializzazione di num_processi
num_processi = 10;

//Inizializzazione delle shared memories
id_buffer = shmget(chiave_buffer_circolare, sizeof(BufferCircolare), IPC_CREAT|0664);
printf("id_buffer = %d.\n", id_buffer);

//Attach delle shared memories
B1 = (BufferCircolare *) (shmat(chiave_buffer_circolare, 0, 0));
B1->testa = 0;
B1->coda = 1;

int p;

for(p = 0; p < 2; p++)
	B1->elementi[i] = 0;

//Inizializzazione di testa e coda
//(*testa) = 0;
//(*coda) = 0;

//Inizializzazione del semaforo
semaphore = semget(chiave_sem, 4, IPC_CREAT|0664);	//Rimettere a 6

//Valori iniziali dei semafori
semctl(chiave_sem, MUTEXP, SETVAL, 1);
semctl(chiave_sem, MUTEXC, SETVAL, 1);
semctl(chiave_sem, spazio_disponibile, SETVAL, 2);
semctl(chiave_sem, messaggio_disponibile, SETVAL, 0);
/*semctl(chiave_sem, cella_disponibile, SETVAL, 1);		//Prima partiva da 1
semctl(chiave_sem, messaggio_cella, SETVAL, 0);*/

//fork

for(i = 0; i < num_processi-4; i++){			//Togliere il -4

	pid = fork();
	
	if(pid == 0){
	
		if(i < 4){
			//PRODUTTORI
			printf("Sono il figlio produttore numero %d, con pid = %d.\n", i+1, getpid());

			//sleep(5);
			Produzione(semaphore, B1);
			//sleep(2 + rand()%4);

		} else if(i == 4 || i == 5){
			//CONSUMATORI
			printf("Sono il figlio consumatore numero %d, con pid = %d.\n", i-3, getpid());
			
			Consumo(semaphore, B1);

		}/* else {
			//LETTORI
			printf("Sono il figlio lettore numero %d, con pid = %d.\n", i-5, getpid());

			//Lettura(semaphore, ptr_cella);

		}*/

		_exit(0);

	}

}

for(i = 0; i < num_processi; i++){

	pid = wait(&st);

	if(pid == -1){
		perror("Errore! Fork non riuscita! Oh no.\n");

	} else {
		printf("Sono il processo padre! Sto aspettando i miei figli.\n");
		printf("Il figlio %d è morto con stato %d!\n", pid, st>>8);
	}

}

//Cancellazione dei buffer e dei semafori

shmctl(id_buffer, IPC_RMID, 0);
semctl(semaphore, 0, IPC_RMID);

return 0;
}
