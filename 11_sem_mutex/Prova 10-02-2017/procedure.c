#include "header.h"

void Signal_Sem (int idsem, int num_sem){

	struct sembuf sem_buf;
	
	sem_buf.sem_num = num_sem;
	
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = 1;
	
	semop (idsem, &sem_buf, 1);

}

void Wait_Sem (int idsem, int num_sem, int var){
	
	struct sembuf sem_buf;
	
	sem_buf.sem_num = num_sem;
	
	sem_buf.sem_flg = 0;
	sem_buf.sem_op = var;
	
	semop (idsem, &sem_buf, 1);


}


int Ricerca_Minimo_Figlio (int *v, int i){

	int min = -1;
	
	for (int t=(i*1000); t<(i*1000)+1000; t++){
	
		if (min<v[t]){
		
			min = v[t];
		
		}	
		
	
	}
	
	printf ("FIGLIO <%d> - MAX: %d\n", getpid(), min);
	
	for (int t=(i*1000); t<(i*1000)+1000; t++){
	
		if (min>v[t]){
		
			min = v[t];
		
		}	
		
	
	}
	
	
	return min;


}


void Produzione_Minimo (Buffer *b, int *v, int idsem, int i){
	
	Wait_Sem (idsem, MUTEX);

	int min = Ricerca_Minimo_Figlio (v, i);
	
	printf ("FIGLIO <%d> - Minimo trovato : %d\n", getpid(), min);
	
	printf ("FIGLIO <%d> - Inserimento Minimo nel buffer...\n", getpid());
	
	b->v_min [i] = min;
	
	printf ("FIGLIO <%d> - BUFFER [%d]: %d\n", getpid(), i, b->v_min [i]);
	
	printf ("FIGLIO <%d> - Minimo inserito nel buffer!\n\n\n", getpid());
		
	Signal_Sem (idsem, CONS);
	
	Signal_Sem (idsem, MUTEX);


}


int Ricerca_Minimo_Buffer (Buffer *b){

	int min = -1;
	
	for (int i=0; i<10; i++){
	
		if (min < b->v_min[i]){
		
			min = b->v_min[i];
		
		}
	
	}
	
	for (int i=0; i<10; i++){
	
	
		if (min > b->v_min [i]){
		
			
			min = b->v_min [i];
	
		}
	}

	return min;
}

void Leggi_Buffer (Buffer *b){

	for (int i=0; i<10; i++){
	
	printf ("BUFFER [%d]: %d\n", i, b->v_min[i]);
	
	}

}

void Consumo_Minimo (Buffer *b, int idsem){
	
	Wait_Sem (idsem, CONS, -10);

	printf ("PADRE <%d> - Lettura buffer in corso...\n", getpid());
	
	Leggi_Buffer (b);
	
	int min;
	
	printf("PADRE <%d> - Ricerca del minimo in corso...\n", getpid());
	
	min = Ricerca_Minimo_Buffer (b);

	printf ("PADRE <%d> - Minimo Trovato: %d\n", getpid(), min);

}
