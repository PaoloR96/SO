#include "procedure.h"

int main(int argc, char * argv[]){
	
	int i,pid;
	msg m;
	int ret;
	

	//controllo sugli argomenti passati 
	if(argc<2){
		printf("ERRORE INSERIMENTO: inserisci 2 valori separati da spazio...\n");
		_exit(0);

	}
	//passo valori da tastiera

	char primo_termine=*argv[1];	
	char secondo_termine=*argv[2];

	//creo chiavi da valori passati
	
	key_t chiave_coda_a=ftok(".",primo_termine);
	key_t chiave_coda_b=ftok(".",secondo_termine);
	
	//creo code con chiave generata da paramentri passati da tastiera

	coda_a=msgget(chiave_coda_a,IPC_CREAT|0664);
	if(coda_a==-1) perror("CODA A errore creazione");

	coda_b=msgget(chiave_coda_b,IPC_CREAT|0664);
	if(coda_b==-1) perror("CODA B errore creazione");

	//genero processi 

	pid=fork();
	if(pid==0){
		mittente();
		_exit(0);	
	}
	
	pid=fork();
	if(pid==0){
		destinatario();
		_exit(0);	
	}

	//padre in attesa 
	for(i=0;i<2;i++){
		pid=wait(0);
	}
	

	//dealloco
	msgctl(coda_a,IPC_RMID,0);
	msgctl(coda_b,IPC_RMID,0);


return 0;
}
