#ifndef tlib
#define tlib

//definisco il numero di thread
#define NUM_THREADS 10

//definisce lo stato della della nostra struttura
enum{VUOTO,PIENO};

//definisco l'accesso alla risorsa in modo esclusivo (mutua esclusione)
pthread_mutex_t MUTEX;

//definisco delle varaibili condition per l'accesso al buffer tra solo scrittori e soli consumatori
pthread_cond_t OK_CONSUMA;
pthread_cond_t OK_PRODUCI;

//struttura da utilizzare
typedef struct{
	int dato;
	int stato;
}Buffer;

//procedure gestione produttore e consumatore
void inizio_Produzione(Buffer*);
void fine_Produzione(Buffer*);
void inizio_Consumazione(Buffer*);
void fine_Consumazione(Buffer*);

//procedure che invocheranno i thread
void *Consumatore(void*);
void *Produttore(void*);

#endif
