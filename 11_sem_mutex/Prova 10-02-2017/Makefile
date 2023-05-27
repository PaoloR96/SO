all:start

start: main.o procedure.o
	g++ -o start main.o procedure.o
	
procedure.o: procedure.c header.h
	g++ -c procedure.c

main.o: main.c header.h
	g++ -c main.c
