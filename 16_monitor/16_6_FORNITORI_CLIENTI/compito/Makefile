all: master

master: procedure.o master.o ../monitor/monitor.o
	gcc -o master procedure.o master.o ../monitor/monitor.o

procedure.o: procedure.c header.h 
	gcc -c procedure.c

master.o: master.c header.h 
	gcc -c master.c

clean:
	rm -f *.o
	rm -f ./master


