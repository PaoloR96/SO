all:start

start: lib.o main.o
	g++ -o start lib.o main.o

lib.o: lib.c lib.h
	g++ -c lib.c

main.o: main.c lib.h
	g++ -c main.c

clean:
	rm -f *.o
	rm -f ./start
	rm -f *~
