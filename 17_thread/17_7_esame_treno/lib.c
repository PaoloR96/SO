#include "lib.h"

void *viaggiatori(void *p){
	//gli passo l'intera struttura monitor e poi vado a selezionare quello che mi serve
	monitor_treno *m = (monitor_treno*)p;
	int i;
	int pos = 0;
	int delay = 0;
	
	//genero un numero random che sara il treno in cui lo scrittore andra a leggere per sempre
	int treno = rand()%4;
	
	for(i=0; i<3; i++){
		delay = (1+rand()%5);
	
		//lo scrittore va a leggere sempre sullo stesso treno
		pos = leggi_stazione(&m[treno]);
		printf("leggo stazione %d su treno %d\n", pos, treno);
		sleep(delay);
	}
	pthread_exit(0);
}

void *capo_treno(void * p){
	monitor_treno *m = (monitor_treno*)p;
	int i;
	int stazione = 0;
	for(i=0; i<10; i++){
		stazione++;
		scrivi_stazione(m,stazione);
		sleep(3);
	}
	pthread_exit(0);
}

void inizializza(monitor_treno*m){
	int i;

	//inizializzo le variabili 
	for(i=0; i<NUM_THREADS; i++){
		
		m[i].stazione = 0;
		m[i].num_lettori = 0;
		m[i].i = 0;
		m[i].stato = LIBERO;
		
		//inizializzo i monitor 
		pthread_mutex_init(&m[i].mutex, NULL);
		pthread_cond_init(&m[i].leggi, NULL);
		pthread_cond_init(&m[i].scrivi, NULL);
	}
}

void rimuovi(monitor_treno *m){
	int i;

	//rimuovo le strutture create
	for(i=0; i<NUM_THREADS; i++){
		printf("rimuovo \n");
		pthread_mutex_destroy(&m[i].mutex);
		pthread_cond_destroy(&m[i].leggi);
		pthread_cond_destroy(&m[i].scrivi);
	}
	printf("rimuovo monitor_treno\n");
	free(m);
}

//si comporta come i lettori
int leggi_stazione(monitor_treno *m){
	int i;
	
	pthread_mutex_lock(&m->mutex);
	while(m->stato == OCCUPATO && m->num_lettori == 0){
		pthread_cond_wait(&m->leggi, &m->mutex);
	}
	m->stato = OCCUPATO;
	m->num_lettori++;
	pthread_cond_signal(&m->leggi);
	pthread_mutex_unlock(&m->mutex);
	
	i = m->stazione;
	
	pthread_mutex_lock(&m->mutex);
	m->num_lettori--;
	if(m->num_lettori == 0){
		m->stato = LIBERO;
		pthread_cond_signal(&m->scrivi);
	}
	pthread_mutex_unlock(&m->mutex);
	
	return i;
}

//si comporta da scrittore
void scrivi_stazione(monitor_treno *m, int stazione){
	
	pthread_mutex_lock(&m->mutex);
	while(m->stato == OCCUPATO){
		pthread_cond_wait(&m->scrivi ,&m->mutex);
	}
	m->stato = OCCUPATO;
	//in questo casonon c'è bisogno di uscire dal monitor
	//pthread_mutex_unlock(&m->mutex);
	
	m->stazione  = stazione;
	printf("scrivo stazione %d su treno %d \n", m->stazione, m->i);
	
	//pthread_mutex_lock(&m->mutex);
	m->stato = LIBERO;
	pthread_cond_signal(&m->leggi);
	pthread_mutex_unlock(&m->mutex);	
}


