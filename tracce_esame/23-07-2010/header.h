/*
Applicazione multiprocesso basata su code di messaggi UNIX.

Un processo Server attende messaggi da un gruppo di Client.
Il Server accumula in un buffer; quando riceve 10 messaggi, li manda ad un terzo processo, Printer, il quale stampa a video i messaggi ricevuti insieme ad un progressivo.
I messaggi contengoono il PID del Client, e ciascun Client genera 15 messaggi, aspettando un secondo fra un messaggio e l'altro.

Un unico main istanzia 5 client, server, printer.
*/

#ifndef HEADER_H
#define HEADER_H

/* msg queues */
#define SERVER_QUEUE 1
#define PRINTER_QUEUE 2

/* Size of server buffer and how much message to print */
#define SIZE 10

/* How many times a client has to send messages to server */
#define CNT_CLIENT 15

#include <sys/types.h> // pid_t
typedef struct {
	long type;
	pid_t message;
} MessageClientServer;

typedef struct {
	long type;
	MessageClientServer array[SIZE];
} MessageServerPrinter;

void client(int msg_queue);
void server(int msg_queue);
void printer(int msg_queue);

#endif /* HEADER_H */
