ESEG = prod

SRC = main.c
SRCOBJ = main.o

HDR = semaphore.h
HDRSRC = semaphore.c
HDROBJ = semaphore.o

all: $(ESEG)

$(ESEG): $(SRCOBJ) $(HDROBJ)
	gcc -o $(ESEG) $(SRCOBJ) $(HDROBJ)

$(SRCOBJ): $(SRC) $(HDR)
	gcc -o $(SRCOBJ) -c $(SRC)

$(HDROBJ): $(HDR) $(HDRSRC)
	gcc -o $(HDROBJ) -c $(HDRSRC)

clean:
	rm -f *.o
	rm -f $(ESEG)

