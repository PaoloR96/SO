MNTLIB = ../monitor/monitor.o

all: start

start: lib.o main.o $(MNTLIB)
	gcc -o exe main.o lib.o $(MNTLIB)

lib.o: lib.c lib.h
	gcc -c -DDEBUG_ lib.c

main.o: main.c lib.h
	gcc -c -DDEBUG_ main.c

clean:
	rm -f *.o
	rm -f exe
