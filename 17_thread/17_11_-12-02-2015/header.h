#include <pthread.h>

#define VUOTO 0
#define IN_USO 1
#define PIENO 2
#define NUM_THREADS 6

typedef struct {
	int operandi[4];
	int totale_operandi;
} Buffer;

typedef struct {
	int cellevuote;
	int cellepiene;
	Buffer elaborazioni[5];
	pthread_cond_t ok_produzione;
	pthread_cond_t ok_consumo;
	pthread_mutex_t MUTEX;
	int VettStato[5];
} MonitorElaborazioni;

void *Produttore(void *);
int InizioProd(MonitorElaborazioni*);
void FineProd(MonitorElaborazioni*, int);
void *Consumatore(void *);
int InizioCons(MonitorElaborazioni*);
void FineCons(MonitorElaborazioni*, int);


