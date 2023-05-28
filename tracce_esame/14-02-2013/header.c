#include "header.h"
#include <sys/sem.h>

void sem_op(int sem_id, int sem_num, int val){
	struct sembuf s;
	
	s.sem_num = sem_num;
	s.sem_flg = 0;
	s.sem_op = val;
	
	semop( sem_id, &s, 1 );
}

void wait_sem(int sem_id, int sem_num){
	sem_op(sem_id, sem_num, -1);
}

void signal_sem(int sem_id, int sem_num){
	sem_op(sem_id, sem_num, +1);
}

void inizio_scrittura(int sem_id, Esame *e){
	wait_sem(sem_id, mutex);
	
	while(1){
		if( e->num_lettori > 0 ){
			signal_sem(sem_id, mutex); // rilascia il mutex
			wait_sem(sem_id, mutex_occupato); // aspetta che sia sbloccato il mutex dell'occupato
			wait_sem(sem_id, mutex); // riacquista il mutex
		} else break;
	}

	wait_sem(sem_id, mutex_occupato);
	e->occupato = 1;
	signal_sem(sem_id, mutex_occupato);
}

void fine_scrittura(int sem_id, Esame *e){
	wait_sem(sem_id, mutex_occupato);
	e->occupato = 0;
	signal_sem(sem_id, mutex_occupato);
	
	signal_sem(sem_id, mutex);
}

void professore(int sem_id, Esame *e){
	static int count = 1;
	string s;
	
	switch (count){
		case 1:
		s = "Prima data disponibile";
		break;
		
		case 2:
		s = "Seconda data disponibile";
		break;
		
		case 3:
		s = "Terza data disponibile";
		break;
		
		default:
		s = "Quarta o piu data disponibile";
		break;
	}
	
	count += 1;
	
	inizio_scrittura(sem_id, e);
	
	e->data_prossimo_appello = s;
	e->num_studenti_prenotati = 0;
	
	fine_scrittura(sem_id, e);
}

void studente(int sem_id, Esame *e){
	wait_sem(sem_id, mutex_num);
	e->num_studenti_prenotati +1 =;
}
