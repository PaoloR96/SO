/*PROCESSO CONSUMATORE*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"


int main(){
	printf("[Consumatore] Sono il processo consumatore con pid ",getpid());
	return 1;
}
