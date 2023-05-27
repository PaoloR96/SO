all:start

start: procedure.o main.o
	gcc -pthread -o start procedure.o main.o

procedure.o: procedure.c thread.h
	gcc -c procedure.c

main.o: main.c thread.h
	gcc -c main.c

clean:
	rm -f ./start
	rm -f *.o
	rm -f *~
