#define N 5
#define PATH_SHM "."
#define CHAR_SHM 'a'
#define PATH_SEM "."
#define CHAR_SEM 'b'
#define MUTEXP 0
#define SPAZIO_DISP 1
#define MESS_DISP 2

typedef struct {

	int elementi[N];
	int testa;		//Li usa come indici, non come puntatori
	int coda;

} BufferCircolare;

void Wait_Sem(int, int, int);
void Signal_Sem (int, int, int);
double media(int*);
double varianza(int*, double);
void produci_elemento(int, BufferCircolare*);
void conusma_elementi(int, BufferCircolare*);
