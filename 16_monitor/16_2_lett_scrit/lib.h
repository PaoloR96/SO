#ifndef _MONITOR_L_S_
#define _MONITOR_L_S_

//numero di processi da creare
#define NUM_PROC 10

//richiamo die semafori per gestire i lettori e scrittori
enum{ SYNCH_LETTORI, SYNCH_SCRITTORI };

//struttura dati da utilizzare
typedef struct {
	int dato;
	int num_lettori;
	int occupato;
	Monitor monitor;
}MonitorLettScritt;

#define NUM_CONDITIONS 2

//procedure per sincronizzazione tra processi non ho 
//bisogno di altri parametri sono gia presenti nella struct
void Lettore(MonitorLettScritt*);
void Scrittore(MonitorLettScritt*);
void InizioLettura(MonitorLettScritt*);
void InizioScrittura(MonitorLettScritt*);
void FineLettura(MonitorLettScritt*);
void FineScrittura(MonitorLettScritt*);


#endif /* _MONITOR_L_S */
