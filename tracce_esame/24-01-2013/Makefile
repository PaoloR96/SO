all: eseg

eseg: main.c header.o
	gcc -o $@ $^ -lpthread

clean:
	rm -f *.o
	rm -f eseg
