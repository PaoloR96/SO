#define MUTEX 0
#define MUTEXL 1
#define SYNCH 2
#define NMAX 5
#define PATH_SHM "."
#define CHAR_SHM 'a'
#define PATH_SEM "."
#define CHAR_SEM 'b'

typedef struct{
	int mat[NMAX][NMAX];
	int dim;
	int condizione;
} Thunderstruct;

void wait_sem(int, int);
void signal_sem(int, int);
void aggiornamento(Thunderstruct*, int);
void raddoppio(Thunderstruct*, int);
void media(Thunderstruct*, int);

