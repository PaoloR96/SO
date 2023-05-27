all : start
	
start: ./Monitor/monitor.o procedure.o main.o 
	g++ -o start ./Monitor/monitor.o procedure.o main.o 

procedure.o: procedure.c ./Monitor/monitor.h mialib.h
	g++ -c procedure.c

main.o : main.c ./Monitor/monitor.h mialib.h
	g++ -c main.c

clean: 
	rm -f *.o
	rm -f *~
	rm -f ./start
