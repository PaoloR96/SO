all: start

start: lib.o main.o ./Monitor/monitor.o
	g++ -o start lib.o main.o ./Monitor/monitor.o

lib.o: lib.c lib.h
	g++ -c lib.c

main.o: main.c lib.h
	g++ -c main.c

clean:
	rm -f *.o
	rm -f *~
	rm -f ./start
