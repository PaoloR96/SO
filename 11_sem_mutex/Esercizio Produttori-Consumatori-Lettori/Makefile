all:  buffercircolare

buffercircolare: buffercircolare.o procedure.o
	gcc -o start buffercircolare.o procedure.o

buffercircolare.o: buffercircolare.c header.h
	gcc -c buffercircolare.c
   
procedure.o: procedure.c header.h
	gcc -c procedure.c
      
clean:
	rm -f *.o 
	rm -f ./start
	rm -f *.kdev*
	rm -f *~
