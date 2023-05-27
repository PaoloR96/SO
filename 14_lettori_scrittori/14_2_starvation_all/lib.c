#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "lib.h"

//gestione semafori
void signal(int id_sem, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = 1;
	semop(id_sem, &sem, 1); //semaforo verde

}

void wait(int id_sem, int num_sem){
	struct sembuf sem;
	sem.sem_num = num_sem;
	sem.sem_flg = 0;
	sem.sem_op = -1;
	semop(id_sem, &sem, 1); //semaforo rosso
}

//gestione lettura e scrittura
void inizioLettura(int id_sem, Buffer *buffer){
	
	//entro in mutua esclusione tra i lettori per modificare le varibiali buffer->num_lettori, buffer->cont_lettura
	wait(id_sem, MUTEX_LETTORI);
	buffer->num_lettori++;
	buffer->cont_lettura++;

	//controllo se sono il primo lettore blocco gli scrittori wait(SYNCH) e attendo una signal(SYNCH) se il semaforo è rosso	
	if(buffer->num_lettori == 1){
		wait(id_sem, SYNCH);	
	}

	//avverto i lettroi che ho terminato
	signal(id_sem, MUTEX_LETTORI);
}

void fineLettura(int id_sem, Buffer *buffer){
	
	//entro di nuovo in muta esclusione tra i lettori MUTEX_LETTORI	modifico buffer->num_lettori
	wait(id_sem, MUTEX_LETTORI);
	buffer->num_lettori--;

	//se il numero di lettori presenti nella risorsa è 0, mando una signal su SYNCH che mi
	//permette di far entrare lettori o scrttori a sorte	
	if(buffer->num_lettori == 0){
		signal(id_sem, SYNCH);	
	}

	//segnalo ai lettori che ho finito MUTEX_LETTORI
	signal(id_sem, MUTEX_LETTORI);
}

void inizioScrittura(int id_sem, Buffer *buffer){
	
	//blocco gli altri scrittori per modificare le variabili buffer->num_scrittori, buffer->cont_scrittura
	wait(id_sem, MUTEX_SCRITTORI);
	buffer->num_scrittori++;
	buffer->cont_scrittura++;

	//se sono il primo scrittore blocco gli altri e attendo SYNCH e me la prendo blocco sia scrittori che lettori	
	if(buffer->num_lettori == 1){
		wait(id_sem, SYNCH);	
	}
	
	//dico agli alti scrttori che ho temrinato su buffer->num_scrittori, buffer->cont_scittura
	signal(id_sem, MUTEX_SCRITTORI);

	//blocco la variabile dato tra gli scrittori se usavo MUTEX_SCRITTORI bloccavo
	//anche gli scrittori che volevano modifcare buffer->num_lettori
	wait(id_sem, MUTEX);
}

void fineScrittura(int id_sem, Buffer *buffer){

	//rilascio il MUTEX e attendo e blocco gli scrittori MUTEX_SCRITTORI modifico buffer->num_scrittori
	signal(id_sem, MUTEX);
	wait(id_sem, MUTEX_SCRITTORI);
	buffer->num_scrittori--;

	//se non ci sono più scrittori rilascio il SYNCH e se lo prenderà o uno scrittore o un lettore
	if(buffer->num_scrittori == 0){
		signal(id_sem, SYNCH);
	}	

	//segnalo agli scrittori che ho temrinato 
	signal(id_sem, MUTEX_SCRITTORI);
}

//Lettori e scrittori
void Lettore(int id_sem, Buffer *buffer){
	inizioLettura(id_sem, buffer);
	
	printf(" Leggo dato [%d] num_lettori <%d> lettura numero <%d>\n", buffer->dato, buffer-> num_lettori, buffer->cont_lettura);

	fineLettura(id_sem, buffer);
}
void Scrittore(int id_sem, Buffer *buffer){
	inizioScrittura(id_sem,buffer);
	
	buffer->dato = rand()%120;
	printf("Dato prodotto <%d> num_scrittori <%d> scrittura numero <%d>\n", buffer->dato, buffer-> num_scrittori, buffer->cont_scrittura);	
	
	fineScrittura(id_sem,buffer);

}
