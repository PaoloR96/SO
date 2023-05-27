all: start

start: thread.o main.o
	gcc -pthread -o start thread.o main.o
	
thread.o: thread.c thread.h
	gcc -c thread.c
	
main.o: main.c thread.h
	gcc -c main.c
	
clean:
	rm -f ./start
	rm -f *.o
	rm -f *~