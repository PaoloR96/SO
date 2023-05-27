all: main

main: main.o procedure.o
	gcc -pthread -o main main.o procedure.o

procedure.o: procedure.c header.h
	gcc -c -o procedure.o procedure.c

main.o: main.c header.h
	gcc -c -o main.o main.c

clean:
	rm -f *.o
	rm -f ./main
