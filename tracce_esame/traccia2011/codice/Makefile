# flag for removing sleep inside main.c
FAST = #-DFAST_

all: eseg

eseg: main.c header.o ../monitor/monitor.o
	gcc $(FAST) -o $@ $^
	mv $@ ../$@

clean:
	rm -f *.o
	rm -f ../eseg
