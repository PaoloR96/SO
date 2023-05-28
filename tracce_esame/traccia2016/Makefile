# remove # below to enable debug flag
DBG = #-DDEBUG
# eseg to generate
ESEG = main generatore elaboratore analizzatore

all: $(ESEG)

main: main.c header.o
	gcc $(DBG) -o $@ $^
generatore: generatore.c header.o
	gcc $(DBG) -o $@ $^
elaboratore: elaboratore.c header.o
	gcc $(DBG) -o $@ $^
analizzatore: analizzatore.c header.o
	gcc $(DBG) -o $@ $^

.PHONY:	clean

clean:
	rm -f *.o
	rm -f $(ESEG)
