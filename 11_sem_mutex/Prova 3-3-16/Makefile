all:  main

main:  analizzatore elaboratore generatore
	gcc -g -o main main.c

analizzatore:
	gcc -o analizzatore analizzatore.c procedure.c -lm    

elaboratore:
	gcc -g -o elaboratore elaboratore.c procedure.c -lm

generatore:
	gcc -g -o generatore generatore.c procedure.c -lm

clean:
	rm -f *.o 
	rm -f ./main ./analizzatore ./elaboratore ./generatore
