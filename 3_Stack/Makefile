all:start

start:  mialib.o Stack.o
	g++ -o start mialib.o Stack.o

miaLib.o: mialib.c mialib.h
	g++ -c mialib.c

main.o:	Stack.c mialib.h
	g++ -c Stack.c
