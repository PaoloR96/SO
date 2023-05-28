all: eseg

eseg: main.o header.o
	gcc -pthread -o $@ $^

clean:
	rm -f *.o
	rm -f eseg
