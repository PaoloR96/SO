all: start

start: main.o procedure.o
	gcc -o start main.o procedure.o

main.o: main.c procedure.h
	gcc -c main.c
	
procedure.o: procedure.c procedure.h
	gcc -c procedure.c

clean: 
	rm -f *.o 
	rm ./start 
