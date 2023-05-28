#ifndef HEADER
#define HEADER

#define NUM_LETTORI 10
#define NUM_SCRITTORI 1
#define NUM_PROCESSI NUM_LETTORI+NUM_SCRITTORI

#define NUM_SCRITTURE 20
#define NUM_LETTURE 10

#define NUM_VAR 2
#define CAN_WRITE 0
#define CAN_READ 1

#include "../monitor/monitor.h"

typedef struct {
	int temperatura; // fra -50 e 50
	unsigned int umidita; // fra 0 e 100, percentuale
	unsigned short pioggia; // 1 piove 0 non piove, bool
} Meteo;

typedef struct {
	Meteo m;
	unsigned short countReaders;
	unsigned short countWriters;
	unsigned short busy;
} Struttura;

void scritturaMeteo(Monitor *mPtr, Struttura *sPtr);
void letturaMeteo(Monitor *mPtr, Struttura *sPtr);

#endif
