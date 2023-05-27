
//e la stessa cosa che fare define solo che cosi li assegna automaticamente
enum{ SPAZIO_DISP, MSG_DISP, MUTEX };

/*
#define SPAZIO_DISP 0
#define MSG_DISP 1
#define MUTEX 2
*/

//numero di processi da creare
#define NUM_PROC 6

//dimensione del nostro array
#define DIM 5


//struttura dati da usare 
typedef struct{
	//buffer
	int vettore[DIM];
	int testa;
	int coda;
}Buffer;

//gestione semafori
void wait_sem(int,int,int);
void signal_sem(int,int,int);

//gestione produttore e consumatore
void produttore(int,Buffer*);
void consumatore(int,Buffer*);
