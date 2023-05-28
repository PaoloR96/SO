DBG = -DDEBUG

HDR = header
HOBJ = $(HDR).o
HSRC = $(HDR).c
HHDR = $(HDR).h

ESEG = start mediatore generatore

all: $(ESEG)

start: start.c $(HOBJ)
	gcc -o $@ $^

mediatore: mediatore.c $(HOBJ)
	gcc -o $@ $^

generatore: generatore.c $(HOBJ)
	gcc -o $@ $^

$(HOBJ): $(HSRC) $(HHDR)
	gcc $(DBG) -o $@ -c $<

###

.PHONY: clean

clean:
	rm -f *.o
	rm -f $(ESEG)

