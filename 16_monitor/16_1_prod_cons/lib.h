#ifndef __mylib__
#define ___mylib__

//numero di processi
#define NUM_PROC 10

//sono semafori usati nel Monitor, enum da lui i valori al posto di #define
enum {OK_PRODUZIONE, OK_CONSUMO};

//questo è il buffer condiviso dove i porduttori inseriranno e consumatori consumeranno
typedef int Buffer;

//per la sincronizzazione tra lettori e scrittori abbiamo bisogno nella memoria condivisa
//anche di questa struct Condition che ci da un controllo su quando consumare e quando produrre vedere nelle funzioni l'uso
typedef struct{
	int ok_produzione;
	int ok_consumo;
}Cond;

//numero di condition necessarie da mettere nell'inizializzazione del Monitor ( init_monitor(); ).
#define NUM_CONDITIONS 2

//procedure di conumazione e produzione da parte dei vari processi
void consumatore(Monitor*,Cond*,Buffer*);
void produttore(Monitor*,Cond*,Buffer*);

//procedure per la sincronizazione dei processi
void InizioConsumo(Monitor*,Cond*);
void InizioProduzione(Monitor*,Cond*);
void FineConsumo(Monitor*,Cond*);
void FineProduzione(Monitor*,Cond*);


#endif /* __mylib__ */
