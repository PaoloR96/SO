all:start

start:  miaLib.o main.o
	g++ -o start miaLib.o main.o

miaLib.o: miaLib.c miaLib.h
	g++ -c miaLib.c

main.o:	main.c miaLib.h
	g++ -c main.c
