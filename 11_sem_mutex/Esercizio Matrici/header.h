#define VUOTO 2
#define PIENO 3
#define IN_USO 4
#define dmax2 20
#define MUTEXP 0
#define spazio_disp 1
#define semaforzero 0

//int indicecolonna = 0;	//è legale?

typedef int msg;

void scrivi_matrice(int n, int m, int [][dmax2]);
void leggi_matrice(int n, int m, int [][dmax2]);

void Wait_Sem(int, int);
void Signal_Sem (int, int);
void WaitForZero_Sem(int, int);

void somma_colonna(int*, msg*, int, int, int, int, int [][dmax2]);
int RichiestaS(int*, int, int);
void Sommatoria(int, msg*, int [][dmax2], int, int);
void RilascioS(int, int*, int);

void somma_totale(msg*, int, int);
