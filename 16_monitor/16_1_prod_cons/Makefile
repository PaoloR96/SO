all: start

start: lib.o main.o monitor.o
	g++ -o start lib.o main.o monitor.o

monitor.o: monitor.c monitor.h
	g++ -c monitor.c 

lib.o: lib.c lib.h monitor.h
	g++ -c lib.c 

main.o: main.c lib.h monitor.h
	g++ -c main.c 

clean:
	rm -f *.o
	rm -f ./start
	rm -f *~
