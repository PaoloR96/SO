all: start

start:  main.o thread.o
	g++ -pthread -o start  main.o thread.o

main.o: main.c thread.h
	g++ -c main.c
		
thread.o: thread.c thread.h
	g++ -c thread.c
	
clean:
	rm -f ./start
	rm -f *.o
	rm -f *~
