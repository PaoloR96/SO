#define NUM_PROC 11

#define SYNCL 0
#define SYNCS 1

#define NUM_COND 2

typedef struct{
	int temperatura;
	unsigned int umidita;
	unsigned short pioggia;
} Meteo;

typedef struct{
	Meteo meteo;
	int numLettori;
	int numScrittori;
	Monitor m;
} Bs;	//Buffer Struct

void servizio(Bs*);
void utente(Bs*);
