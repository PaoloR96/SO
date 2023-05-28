all: eseg

eseg: main.o header.o
	gcc -o $@ $^
	
clean:
	rm -f *.o
	rm -f eseg
