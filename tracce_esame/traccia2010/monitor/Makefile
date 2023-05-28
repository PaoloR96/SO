DBG = #-DDEBUG_

all: monitor.o

monitor.o: monitor.c monitor.h
	gcc $(DBG) -c monitor.c
clean:
	rm -f *.o
	rm -f *~
	rm -f *kdev*

