FLG = -pthread

all: eseg

eseg: main.o header.o
	gcc $(FLG) -o $@ $^
	
clean:
	rm -f *.o
	rm -f eseg
