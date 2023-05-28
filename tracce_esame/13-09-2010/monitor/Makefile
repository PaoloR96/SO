all: eseg

FAST_FLG = -DFAST

eseg: monitor.o header.o main.o
	gcc $(FAST_FLG) -o $@ $^

clean:
	rm -f *.o
	rm -f *~
	rm -f *kdev*
