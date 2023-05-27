
//semafori
#define SPAZIO_DISP  0
#define MSG_DISP  1
#define MUTEX_PROD 2
#define MUTEX_CONS 3 

//numero operazioni da esegire 
#define NUM_PRODUZIONI 4
#define NUM_CONSUMAZIONI 4

//dimensione buffer
#define DIMENSIONE 5

typedef int msg;

void WaitSem(int, int );
void SignalSem (int, int );

void Produttore(int*,msg*,int);
void Consumatore(int*,msg *,int);
