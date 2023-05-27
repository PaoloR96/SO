all:  start

start:  produttore consumatore
	gcc -g -o start main.c -lm

produttore: produttore.o lib.o 
	g++ -o produttore produttore.o lib.o

produttore.o: produttore.c
	g++ -c produttore.c lib.h  

lib.o: lib.c
	g++ -c lib.c lib.h

consumatore: consumatore.o lib.o
	g++ -o consumatore consumatore.c lib.o

consumatore.o: consumatore.c lib.c
	g++ -c consumatore.c lib.h

clean:
	rm -f *.o 
	rm -f ./start ./consumatore ./produttore
	rm -f *~

