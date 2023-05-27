#include "header.h"

void InizializzaMagazzino (Magazzino *g){

	int i;
	
	for (i=0; i<10; i++){
	
	printf ("INIZIALIZZAZIONE PRODOTTO [%d]\n",i);
	
	g[i]->costo = rand ()%5+1;
	
	printf ("[%d] COSTO = %d\n", i, g[i]->costo);
	
	g[i]->quantita = rand ()%5+1;
	
	printf ("[%d] QUANTITA' = %d\n\n", i, g[i]->quantita);
	
	}
	
	
	printf ("Magazzino inizializzato.\n\n");
}


void Client (int id_ric, int id_ris){

	Messaggio m;
	Risposta r;
	RispostaAcquisto ra;
	
	int i;
	
	m.tipo = LTT;
	m.pid = getpid ();
	m.prodotto = 0;
	
	msgsnd (id_ric, &m, sizeof (Messaggio) - sizeof (long), 0);
	
	printf ("CLIENT <%d> - Messaggio di lettura inviata!\n", getpid());


	msgrcv (id_ris, &r, sizeof (Risposta) - sizeof (long), getpid(), 0);
	
	printf ("CLIENT <%d> - Risposta di lettura ricevuta!\n", getpid());
	for (i=0; i<10; i++){
		
		printf ("PRODOTTO [%d]\n",i);
		printf("COSTO = %d\n", r.costo[i]);
		printf ("QUANTITA' = %d\n\n", r.quantita[i]);
	
	}
	
	m.tipo = ACQ;
	m.pid = getpid ();
	
	for (i=0; i<2; i++){
	
		printf("CLIENT <%d> - RICHIESTA DI ACQUISTO [%d]\n", getpid(), i+1);
		
		m.prodotto = rand ()%10;
		m.quantita = rand ()%5+1;
		
		printf ("CLIENT <%d> - Invio richiesta d'acquisto [%d]!\nPRODOTTO:%d\nQUANTITA':%d\n", getpid(), i+1, m.prodotto, m.quantita);
		
		msgsnd (id_ric, &m, sizeof (Messaggio) - sizeof (long), 0);
		
		msgrcv (id_ris, &ra, sizeof (RispostaAcquisto) - sizeof (long), getpid(), 0);
		
		if (ra.quantita == 0){
			printf ("CLIENT <%d> - Non e' stato possibile effettuare l'acquisto\n", getpid());
		}else {
		
			printf("CLIENT <%d> - Acquisto effettuato correttamente! Quantita acquistata:%d\n", getpid(), ra.quantita);
		
		}
	}
	
	

}


void Fornitore (int id_ric, int id_ris){
	Messaggio m;
	
	m.tipo = FRN;
	
	int i;
	
	for (i=0; i<2; i++){
	
		printf ("FORNITORE <%d> - Fornitura n° %d\n", getpid(), i+1);
		
		m.prodotto = rand ()%10;
		m.quantita = rand ()%5+1;
		
		printf ("FORNITORE <%d> - Invio Fornitura.\nPRODOTTO: %d\nQUANTITA': %d\n", getpid(), m.prodotto, m.quantita);
		
		msgsnd (id_ric, &m, sizeof(Messaggio) - sizeof(long), IPC_NOWAIT);
	
	
	}
	
	printf("\n\n");


}

void Gestore (Magazzino *g, int id_ric, int id_ris){

	Messaggio m;
	Risposta r;
	RispostaAcquisto ra;
	
	int bytes;
	int cont=0;
	
	while(cont<13){
		bytes = msgrcv (id_ric, &m, sizeof (Messaggio) - sizeof (long), LTT, IPC_NOWAIT);//IPC_NOWAIT
	
		if(bytes>0){
			printf ("SERVER <%d> - Messaggio di lettura ricevuto dal Client <%d>\n", getpid(), m.pid);
	
			r.tipo = m.pid;
	
			printf("\n\n");
	
			while (m.prodotto < 10){
		
				r.costo [m.prodotto] = g[m.prodotto]->costo;
				r.quantita [m.prodotto] = g[m.prodotto]->quantita;
				printf ("SERVER <%d> - Informazioni sul prodotto [%d] inserite nella risposta\n", getpid(), m.prodotto);
				m.prodotto ++;
			}
	
			printf("\n\n");
	
			msgsnd (id_ris, &r, sizeof (Risposta) - sizeof (long), 0);
	
			printf ("SERVER <%d> - Risposta al client <%ld> inviata!\n", getpid(), r.tipo);
	
			cont++;
			
			printf ("Cont: %d\n", cont);

		}
		
		bytes = msgrcv (id_ric, &m, sizeof (Messaggio) - sizeof (long), ACQ, IPC_NOWAIT);//IPC_NOWAIT
		
		if(bytes>0){
			printf ("SERVER <%d> - Richiesta di acquisto ricevuta dal Client <%d>.\nPRODOTTO:%d\nQUANTITA':%d\n", getpid(), m.pid, m.prodotto, m.quantita);
	
			ra.tipo = m.pid;
			ra.prodotto = m.prodotto;
			ra.quantita = m.quantita;
			
			printf("\n\n");
			

		if (g[ra.prodotto]->quantita < ra.quantita){
		
			printf ("SERVER <%d> - Impossibile effettuare acquisto: quantita' in magazzino del prodotto [%d] insufficiente\n", getpid (), ra.prodotto);
			
			ra.quantita = 0;
			
			msgsnd (id_ris, &ra, sizeof(RispostaAcquisto)-sizeof(long), 0);
			
			
			cont ++;
			
			printf ("Cont: %d\n", cont);	
		}else{
		
			
			g[ra.prodotto]->quantita = g[ra.prodotto]->quantita - ra.quantita;
			
			printf ("SERVER <%d> - Acquisto effettuato.\nNuova quantita' in magazzino del prodotto [%d]: %d\n", getpid (), ra.prodotto, g[ra.prodotto]->quantita);
			
			msgsnd (id_ris, &ra, sizeof(RispostaAcquisto)-sizeof(long), 0);
			
			cont++;
			
			printf ("Cont: %d\n", cont);
		
		}
	
	}		
	
	bytes = msgrcv (id_ric, &m, sizeof (Messaggio) - sizeof (long), FRN, IPC_NOWAIT);//IPC_NOWAIT
		
	if(bytes>0){
		
		printf ("SERVER <%d> - Fornitura ricevuta dal Fornitore <%d>.\nPRODOTTO: %d\nQUANTITA': %d\n", getpid(), m.pid, m.prodotto, m.quantita);
		
		g[m.prodotto]->quantita = g[m.prodotto]->quantita + m.quantita;
		
		printf("SERVER <%d> - Fornitura completata.\nPRODOTTO: %d\nNUOVA QUANTITA': %d\n\n", getpid(), m.prodotto, g[m.prodotto]->quantita);
	
	cont ++;
	
	printf("Cont: %d\n", cont);
	}
	
	
	}
	
	printf ("Messaggi ricevuti: %d\nChiusura server\n\n\n", cont);
	
}
