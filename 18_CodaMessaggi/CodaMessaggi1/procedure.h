#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#define TIPO 1

int coda_a;
int coda_b;


typedef struct {
	long type;
	char mess[255];
}msg;

void mittente();
void destinatario();



