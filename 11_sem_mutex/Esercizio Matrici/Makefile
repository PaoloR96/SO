all:  matrice

matrice: matrice.o procedurematrice.o
	gcc -o start matrice.o procedurematrice.o

matrice.o:matrice.c header.h
	gcc -c matrice.c
   
procedure.o: procedurematrice.c header.h
	gcc -c procedurematrice.c
      
clean:
	 rm -f *.o 
	rm -f ./start
	rm -f *.kdev*
	rm -f *~
