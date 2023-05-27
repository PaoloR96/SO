#define NUM_MIN 0
#define NUM_MAX 100

#define libero 0
#define occupato 1
#define in_uso 2

#define PATH_SHM "."
#define CHAR_SHM 'a'

#define NUM_CONDITION 2
#define MERCE_DISPONIBILE 0
#define SPAZIO_DISPONIBILE 1

#define NUM_PROCESSI 20
#define NUM_FORNITORI 10
#define NUM_CLIENTI 10

#include "../monitor/monitor.h"

//int livello_scorte;

typedef struct {
	unsigned int id_fornitore;
	unsigned int stato;
} scaffale;

void produci(Monitor*, scaffale*);
int inizio_produzione(Monitor*, scaffale*);
void fine_produzione(Monitor*, scaffale*, int);
void consuma(Monitor*, scaffale*);
int inizio_consumo(Monitor*, scaffale*);
void fine_consumo(Monitor*, scaffale*, int);


