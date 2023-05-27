
//semafori
#define MUTEX_PROD 0
#define MUTEX_CONS 1
#define SPAZIO_DISP 2 
#define MSG_DISP 3

//stato della cella del messaggio
#define VUOTO 0
#define PIENO 1

typedef struct{
	int msg1;
	int msg2;
	int stato_msg1;
	int stato_msg2;
}Buffer;

void inizializza(key_t*,key_t*,Buffer**,int*,int*);
void wait(int,int);
void signal(int,int);
void produttore(Buffer*,int);
void consumatore(Buffer*,int);
void rimozione(int*,int*);
void stampaMemoria(Buffer*);
