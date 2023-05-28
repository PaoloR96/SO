#ifndef HEADER_H
#define HEADER_H

/* Semafori */
#define MUTEX_WRITER_COUNT 0 // init 1
#define MUTEX_READER_COUNT 1 // init 1
#define CANWRITE 2 // init 0 - waitForZero - 0 true, 1 false
#define CANREAD 3 // init 1 - waitForZero - 0 true, 1 false
#define MUTEX_WRITE 4 // init 1
#define MUTEX_PRINTF 5 // init 1

void waitSem(int semid, int semnum);
void signalSem(int semid, int semnum);
void waitForZero(int semid, int semnum);

/* Path */
#define PATH_SEM "."
#define PATH_SHM "."
#define CHAR_SEM 's'
#define CHAR_SHM 'h'

/* Struttura da salvare nella shared memory */
typedef int type;
typedef type matrix[5][5];
typedef struct {
  matrix			m; // matrix structure
  unsigned short int		n; // dimension of matrix
  unsigned short int		countReaders;
  unsigned short int		countWriters;
  unsigned short int		terminate; // 0 normally, 1 when main has to kill sons
} Structure;

/* Funzioni */
// Aggiorna il contenuto della matrice con contenuti random (fra 0 e 9)
void generatore(int semid, Structure *sPtr);
// Legge il contenuto della matrice, moltiplica per due ogni elemento, e stampa a video
void elaboratore(int semid, Structure *sPtr);
// Legge il contenuto della matrice, ne fa il valore medio, e stampa a video
void analizzatore(int semid, Structure *sPtr);

/* Utility */
void inizioLettura(int semid, Structure *sPtr);
void fineLettura(int semid, Structure *sPtr);

void inizioScrittura(int semid, Structure *sPtr);
void fineScrittura(int semid, Structure *sPtr);

#endif
