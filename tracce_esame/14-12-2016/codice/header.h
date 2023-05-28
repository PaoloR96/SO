/*
Produttore consumatore multi-processo con Monitor e SharedMemory
Ufficio postale con N processi cliente e 1 processo sportellista

I processi cliente sono differenziati in cliente_post (postale) e cliente_fin (finanziario)
I processi cliente_fin producono un intero fra 1 e 50, e attendono 4 secondi
I processi cliente_post producono un intero fra 51 e 100, e attendono 2 secondi
Il processo sportellista attende 1 secondo fra un ciclo e l'altro
BUFFER SINGOLO

VINCOLO:
Un cliente_post può produrre solo se non vi è alcun cliente_fin in attesa del buffer

MONITOR SIGNAL-AND-CONTINUE (while)
*/

#ifndef HEADER_
#define HEADER_

#include "../monitor/monitor.h"

#define BUFFER_EMPTY 0
#define BUFFER_FULL 1
#define CLIENT_FIN_OUTSIDE 2

#define NUM_CLIENT_POST 3
#define NUM_CLIENT_FIN 1

#define NUM_CICLI_FIN 4
#define NUM_CICLI_POST 4
#define NUM_CICLI_SPORT (NUM_CLIENT_POST*NUM_CICLI_POST)+(NUM_CLIENT_FIN*NUM_CICLI_FIN) // 16, non 12

typedef struct {
	int job_id; // buffer singolo
	
	Monitor m; // monitor
	
	int is_buffer_full; // cond_var
	int count_client_fin; // cond_var, se >0 i client_post devono attendere che torni a zer
	
} CSMonitor;

void csmonitor_init(CSMonitor *);

void cliente_post(CSMonitor *);
void cliente_fin(CSMonitor *);
void sportellista(CSMonitor *);

void csmonitor_destroy(CSMonitor *);

#endif // Header
