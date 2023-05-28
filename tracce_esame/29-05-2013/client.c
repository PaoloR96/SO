#include "header.h"
#include <unistd.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	key_t key = ftok(PATH_Q, KEY_Q);
	
	int server_qid = msgget(key, 0664);
	if(server_qid < 0){
		fprintf(stderr, "Execute main\n");
		exit(1);
	}
	
	srand(time(NULL));
	
	for(int i = 0; i < NUM_REQ; ++i){
		client( server_qid );
	}
}
