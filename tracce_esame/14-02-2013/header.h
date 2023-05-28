#ifndef HEAD
#define HEAD

/* Applicazione multiprocesso con shared memory e semafori */

typedef string char[200];

typedef struct {
	string data_prossimo_appello;
	int num_studenti_prenotati;
	
	int occupato;
	int num_lettori;
} Esame;

void professore(int, Buffer*);
void studente(int, Buffer*);

#define mutex_occupato 0
#define mutex_num_lettori 1
#define mutex 2

#endif
