#include "lib.h"

int queue_ric,queue_risp;
pthread_t pthread[3];
void *Manager(void *array){
	array_cond *a=(array_cond*)array;
	messaggio m;
	int termina=0;
	while(!termina){
		if(msgrcv(queue_ric,&m,DIM,RICHIESTA,IPC_NOWAIT)==-1)
			printf("MANAGER Messaggio non ricevuto a: %d b: %d\n",m.a,m.b);
		else{
			if(m.a==-1 && m.b==-1){
				//TERMINO
				termina=1;
				pthread_cancel(pthread[1]);
				pthread_cancel(pthread[2]);
				printf("MANAGER Ho cancellato i Worker\n");
			}else{
				printf("MANAGER Ho ricevuto a: %d b: %d da: %d\n",m.a,m.b,m.pid);
				
				//INIZIO PROD CONS
				
				pthread_mutex_lock(&a->MUTEX);
				
				while(a->riempimento==15)
					pthread_cond_wait(&a->OKPROD,&a->MUTEX);

				a->riempimento++;
				a->vet[a->coda]=m;
				printf("MANAGER Ho inserito a: %d b: %d in pos: %d riemp: %d\n",a->vet[a->coda].a,a->vet[a->coda].b,a->coda,a->riempimento);
				a->coda=++a->coda%15;
				pthread_cond_signal(&a->OKCONS);
				pthread_mutex_unlock(&a->MUTEX);
			}
		}
		sleep(1);
	}
	pthread_exit(0);
}

void *Worker(void *array){
	array_cond *a=(array_cond*)array;
	messaggio m;
	while(1){
		pthread_mutex_lock(&a->MUTEX);
		while(a->riempimento==0)
			pthread_cond_wait(&a->OKCONS,&a->MUTEX);
		a->riempimento--;
		m=a->vet[a->testa];
		a->testa=++a->testa%15;
		
		pthread_cond_signal(&a->OKPROD);
		pthread_mutex_unlock(&a->MUTEX);
		
		m.prodotto=m.a*m.b;
		m.tipo=m.pid;
		printf("WORKER Invio prodotto: %d pid: %d\n",m.prodotto,m.pid);
		msgsnd(queue_risp,&m,DIM,0);
		
		
	}
}

int main(){
	int i;
	messaggio m;
	key_t k_ric=ftok(".",'b');
	key_t k_risp=ftok(".",'c');
	//key_t k_s=ftok(".",'a');
	
	//queues=msgget(k_s,IPC_CREAT|0664);
	queue_ric=msgget(k_ric,IPC_CREAT|0664);
	queue_risp=msgget(k_risp,IPC_CREAT|0664);
	
	array_cond *array=(array_cond*)malloc(sizeof(array_cond));
	pthread_mutex_init(&array->MUTEX,NULL);
	pthread_cond_init(&array->OKPROD,NULL);
	pthread_cond_init(&array->OKCONS,NULL);
	array->testa=0;
	array->coda=0;
	array->riempimento=0;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	pthread_create(&pthread[0],&attr,Manager,(void*)array);
	pthread_create(&pthread[1],&attr,Worker,(void*)array);
	pthread_create(&pthread[2],&attr,Worker,(void*)array);
	
	//ATTENDO TERMINAZIONE MANAGER
	pthread_join(pthread[0],NULL);
	
	pthread_cond_destroy(&array->OKPROD);
	pthread_cond_destroy(&array->OKCONS);
	pthread_mutex_destroy(&array->MUTEX);
	free(array);
	//msgctl(queue_ric,0,IPC_RMID);
	//msgctl(queue_risp,0,IPC_RMID);
	
	return 0;
}