#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "lib.h"

//gestione dei semafori (wait e signal classici che vengono effettuati sui semafori a disposizione)
void signal(int id_sem ,int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = 1;
	semop(id_sem, &sem, 1);
}

void wait(int id_sem ,int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = -1;
	semop(id_sem, &sem, 1);
}

//funzioni richiamate dalle funzioni di lettura e scrittura (GUARDARE PRIMA FUNZIONI LETTORE E SCRITTORE)
void inizioLettura(int id_sem, Buffer *buffer){
	
	//inizialmente devo accedre in mutua esclusione tra i lettori perchè vado a modificare un dato in comucne
	//(che midificheranno anche gli altri lettori devo evitare modifiche inconsistenti)
	wait(id_sem, MUTEX_LETTORI);
	buffer->num_lettori++;
	buffer->cont_lettura++;

	//devo leggere controllo se sono il primo lettore vuol dire che devono accedervi anche gli 
	//altri lettori allora blocco, gli scrittori non devono modificare il dato
	if(buffer->num_lettori == 1){
		wait(id_sem, SYNCH);
	}
	
	//segnalo algi altri lettori che ho finito di modificare 
	//il dato num_lettori, cont_lettura, adesso se vogliono possono accedere loro
	signal(id_sem, MUTEX_LETTORI);
}

void fineLettura(int id_sem, Buffer *buffer){
	
	//entro in muta esclusione tra i lettori e visto che ho temirnato di leggere 
	//decremento in valore num_lettori (che mi indica qunati lettori siamo attualemnte ) 
	wait(id_sem, MUTEX_LETTORI);
	buffer->num_lettori--;

	//se siamo scesi a 0 vuol dire che tutti abbiamo letto alora mando una SYNCH che avverte gli scrittori che possono scrivere
	if(buffer->num_lettori == 0){
		signal(id_sem, SYNCH);
	}

	//libero il MUTEX_LETTORI	
	signal(id_sem, MUTEX_LETTORI);
}

//quello che faranno i processi lettore e scrittore
void scrittore(int id_sem, Buffer *buffer){

	wait(id_sem, SYNCH);

	buffer->dato = rand()%120;
	printf("Dato prodotto --> %d\n", buffer->dato);

	signal(id_sem, SYNCH);
}

void lettore(int id_sem, Buffer *buffer){

	inizioLettura(id_sem, buffer);
	
	sleep(1);
	printf("Leggo messaggio [%d] numero lettori <%d>\n", buffer->dato, buffer->num_lettori);

	fineLettura(id_sem, buffer);	
}


