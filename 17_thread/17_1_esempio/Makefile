all:start

start: thread.o
	g++ -pthread -o strat thread.o

thread.o: thread.c
	g++ -c thread.c

clean:
	rm -f ./start
	rm -f *.o
	rm -f *~
