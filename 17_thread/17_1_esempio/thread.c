#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//numero di thread da generare
#define NUM_THREADS 5

//procedura che eseguiranno i thread
void *printHello( void *thread_id){
	printf(" \n HeLLo WoLd !! \n");
	pthread_exit(NULL);
}

int main(){
	
	//threads e un array pari al numero di threads da creare
	pthread_t threads[NUM_THREADS];
	//id della strutura thread che ritornerà
	int rc;
	int i;

	printf("\n\n __ INIZIO__ \n\n");
	
	//genero i threads
	for(i=0; i<NUM_THREADS; i++){
		printf("Creazione Thread %d \n",i);
		//creo i threads printHello funzione che verrà richiamata, void* i 
		//parametro passato se si dovevano passare più parametri si creava
		// una struct e la si passava come paramentro e nella pocedura si
		// faceva un cast inverso.
		rc = pthread_create(&threads[i], NULL, printHello, (void *)(long)i);

		//se rc restituisce un valore diverso da 0 allora la creazione è fallita 
		if(rc != 0){
			
			printf("Errore creazione del thread %d \n",rc);
			exit(-1);
		}
	}

	printf("\n\n __FINE__ \n\n");	

	//terminazione dei thread
	pthread_exit(NULL);
	
	return 0;
}
