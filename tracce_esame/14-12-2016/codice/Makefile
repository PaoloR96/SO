all: eseg

eseg: main.o header.o ../monitor/monitor.o
	gcc -o $@ $^
	
clean:
	rm -f *.o
	rm -f ../monitor/monitor.o
	rm -f eseg
