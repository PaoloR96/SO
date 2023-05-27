/*semplice implementazione del pid, per capire come funzione il pid (process id) ppid(pid del processo padre)*/

#include <stdio.h>
#include <unistd.h>

int main(void){

	int pid;  //processo 
	int ppid; //processo padre

	
	
	pid=getpid();  //assegnaun pid al processo
	printf("\neseguo istruzione pid=getpid(); \n");
	printf("sono il porcesso con il pid : ");
	printf("%d",pid);
	printf("\n\n");

	ppid = getppid(); // resisutisce il pid del processo genitore
	printf("\neseguo istruzione ppid=getppid(); \n");
	printf("il porcesso padre ha pid : ");
	printf("%d",ppid);
	printf("\n\n");

	sleep(5);	//mette in sleep il processo per un tot numero secondi specificato
	return 0;
}
