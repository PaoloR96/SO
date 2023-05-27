all:start

start: mialib.o main.o
	gcc -pthread -o start mialib.o main.o

mialib.o: mialib.c mialib.h
	gcc -c mialib.c
	
main.o: main.c mialib.h
	gcc -c main.c
	
clean:
	rm -f *.o
	rm -f *~
	rm -f ./start
	
	