#include "header.h"
#include <sys/sem.h> // struct sembuf, semop
#include <stdlib.h> // rand, srand, NULL
#include <time.h> // time
#include <stdio.h> // printf
#include <unistd.h> // sleep

void waitSem(int semid, int semnum){
	struct sembuf s;

	s.sem_num = semnum;
	s.sem_flg = 0;
	s.sem_op = -1;

	semop(semid, &s, 1);
}

void signalSem(int semid, int semnum){
	struct sembuf s;

	s.sem_num = semnum;
 	s.sem_flg = 0;
  	s.sem_op = +1;

  	semop(semid, &s, 1);
}

void waitForZero(int semid, int semnum){
 	 struct sembuf s;

  	s.sem_num = semnum;
  	s.sem_flg = 0;
 	s.sem_op = 0;

  	semop(semid, &s, 1);
}

// Aggiorna il contenuto della matrice con contenuti random (fra 0 e 9)
void generatore(int semid, Structure *sPtr){
	inizioScrittura(semid, sPtr);

	sleep(1); // simulo il tempo di routine

	static unsigned int count = 0;

	for(int i = 0; i < sPtr->n; ++i) {
		for(int j = 0; j < sPtr->n; ++j) {
			srand( count );
			type t = rand() % 10; // 0, 1, ..., 9
			(sPtr->m)[i][j] = t;
			count += 1;
		}
	}

	waitSem(semid, MUTEX_PRINTF);
	printf("NUOVA MATRICE:\n");
	for(int i = 0; i < sPtr->n; ++i){
		for(int j = 0; j < sPtr->n; ++j){
			type t = (sPtr->m)[i][j];
			printf("%d\t", t);
		}
		printf("\n");
	}
	signalSem(semid, MUTEX_PRINTF);

	fineScrittura(semid, sPtr);
}
// Legge il contenuto della matrice, moltiplica per due ogni elemento, e stampa a video
void elaboratore(int semid, Structure *sPtr){
	inizioLettura(semid, sPtr);

	waitSem(semid, MUTEX_PRINTF);

	printf("Raddoppio i valori della matrice\n");

	for(int i = 0; i < sPtr->n; ++i){
		for(int j = 0; j < sPtr->n; ++j){
			type temp = (sPtr->m)[i][j];
			printf("%d\t", 2*temp);
		}
		printf("\n");
	}

	signalSem(semid, MUTEX_PRINTF);

	fineLettura(semid, sPtr);
}
// Legge il contenuto della matrice, ne fa il valore medio, e stampa a video
void analizzatore(int semid, Structure *sPtr){
	inizioLettura(semid, sPtr);

	double temp = 0;

	for(int i = 0; i < sPtr->n; ++i){
		for(int j = 0; j < sPtr->n; ++j){
			temp += (sPtr->m)[i][j];
		}
	}

	temp /= (sPtr->n)*(sPtr->n);

	waitSem(semid, MUTEX_PRINTF);
	printf("Valore medio degli elementi della matrice: %lf\n", temp);
	signalSem(semid, MUTEX_PRINTF);

	fineLettura(semid, sPtr);
}

void inizioLettura(int semid, Structure *sPtr){
	// aspetto lo zero (true) per leggere
	waitForZero(semid, CANREAD);

	// incremento il numero di lettori in maniera esclusiva
	waitSem(semid, MUTEX_READER_COUNT);
	sPtr->countReaders += 1;
	signalSem(semid, MUTEX_READER_COUNT);
}
void fineLettura(int semid, Structure *sPtr){
	// decremento il numero di lettori
	waitSem(semid, MUTEX_READER_COUNT);
	sPtr->countReaders -= 1;
	signalSem(semid, MUTEX_READER_COUNT);

	if(sPtr->countReaders == 0){
		// non ci sono più lettori, quindi sblocco gli scrittori
		signalSem(semid, CANREAD); // porta il semaforo da 0 a 1
		waitSem(semid, CANWRITE); // porta il semaforo da 1 a 0
	}
}

void inizioScrittura(int semid, Structure *sPtr){
	// aspetto lo zero (true) per scrivere
	waitForZero(semid, CANWRITE);

	// incremento il numero di scrittori in maniera esclusiva
	waitSem(semid, MUTEX_WRITER_COUNT);
	sPtr->countWriters += 1;
	signalSem(semid, MUTEX_WRITER_COUNT);

	// ottengo l'accesso esclusivo alla risorsa
	waitSem(semid, MUTEX_WRITE);
}
void fineScrittura(int semid, Structure *sPtr){
	// sblocco la risorsa
	signalSem(semid, MUTEX_WRITE);

	// decremento il numero di scrittori
	waitSem(semid, MUTEX_WRITER_COUNT);
	sPtr->countWriters -= 1;
	signalSem(semid, MUTEX_WRITER_COUNT);

	if(sPtr->countWriters == 0){
		// non ci sono più scrittori, quindi sblocco i lettori
		signalSem(semid, CANWRITE); // porta il semaforo da 0 a 1
		waitSem(semid, CANREAD); // porta il semaforo da 1 a 0
	}
}
