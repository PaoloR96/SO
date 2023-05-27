#define MUTEXP 0
#define MUTEXC 1
#define spazio_disponibile 2
#define messaggio_disponibile 3
#define cella_disponibile 4
#define messaggio_cella 5

typedef struct {

	int testa;
	int coda;
	int elementi[2];

} BufferCircolare;

void Signal_Sem(int, int);	//provare con un solo int
void Wait_Sem(int, int);

void Produzione(int, BufferCircolare*);		//semaphore, 
void Consumo(int, BufferCircolare*);
//void Lettura(int, int*);
