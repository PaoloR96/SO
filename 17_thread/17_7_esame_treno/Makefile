all:start

start: main.o lib.o
	gcc -pthread -o start main.o lib.o

main.o: main.c lib.h
	gcc -c main.c

lib.o: lib.c lib.h
	gcc -c lib.c

clean:
	rm -f ./start	
	rm -f *.o
	rm -f *~
