all:start

start:  mialib.o main.o
	g++ -o start mialib.o main.o

miaLib.o: mialib.c mialib.h elemento.h
	g++ -c mialib.c

main.o:	main.c mialib.h elemento.h
	g++ -c main.c
