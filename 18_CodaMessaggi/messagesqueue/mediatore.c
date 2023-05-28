#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "header.h"

int main(int argc, char *argv[]){
	key_t msgKey = ftok(PATH_MSG, CHAR_MSG);
	
	int msgqid = msgget(msgKey, 0);
	if(msgqid < 0){
		fprintf(stderr, "Cannot instantiate message queue! Boot start first\n");
		return 1;
	}
	
	double ret = mediatore(msgqid);
	printf("Media: %lf\n", ret);
}
