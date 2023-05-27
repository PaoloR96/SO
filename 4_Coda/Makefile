all:start

start: mialib.o main.o
	g++ -o start mialib.o main.o

mialib.o: mialib.c mialib.h
	g++ -c mialib.c 

main.o: main.c mialib.h
	g++ -c main.c 


