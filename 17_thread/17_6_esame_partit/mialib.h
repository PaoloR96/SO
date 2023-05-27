#ifndef lib_h
#define lib_h

#define NUM_THREADS 12
enum{LIBERO, OCCUPATO};

typedef struct{
	int goal_a;
	int goal_b;
	int somma_a;
	int somma_b;
	int num_lettori;
	int stato;
	pthread_mutex_t mutex;
	pthread_cond_t leggi;
	pthread_cond_t scrivi;
}buffer;

void *Scrittura_a(void*);
void *Scrittura_b(void*);
void *Lettura(void*);

void goal_a(buffer*);
void goal_b(buffer*);
void utente(buffer*);

#endif