#ifndef MIALIB
#define MIALIB

//semafori
#define SYNCH 0			//serve per semaforizzare l'accesso alla risorsa condivisa tra i processi scrittori e lettori
#define MUTEX_LETTORI 1		//serve per semaforizzare l'accesso sulla variabile buffer->num_lettori, buffer->cont_scrittura
#define MUTEX_SCRITTORI 2	//serve per semaforizzare l'accesso sulla variabile buffer->num_scrittori, buffer->cont_scrittura
#define MUTEX 3			//serve per semaforizzare l'accesso sulla variabile buffer->dato

//numero di processi
#define NUM_PROC 4

//la mia memoria da condividere
typedef struct {
	int dato;
	int num_lettori;
	int cont_lettura;
	int num_scrittori;
	int cont_scrittura;
}Buffer;

//gestione semafori
void signal(int,int);
void wait(int,int);

//gestione lettura e scrittura
void inizioLettura(int,Buffer*);
void fineLettura(int,Buffer*);
void inizioScrittura(int,Buffer*);
void fineScrittura(int,Buffer*);

//Lettori e scrittori
void Lettore(int,Buffer*);
void Scrittore(int,Buffer*);

#endif /* MIALIB */
