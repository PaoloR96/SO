ESEG = prod

SRCNAME = main
SRC = $(SRCNAME).c
SRCOBJ = $(SRCNAME).o

HDRNAME = semaphore
HDR = $(HDRNAME).h
HDRSRC = $(HDRNAME).c
HDROBJ = $(HDRNAME).o

all: $(ESEG)

$(ESEG): $(SRCOBJ) $(HDROBJ)
	gcc -o $(ESEG) $(SRCOBJ) $(HDROBJ)

$(SRCOBJ): $(SRC) $(HDR)
	gcc -o $(SRCOBJ) -c $(SRC)

$(HDROBJ): $(HDR) $(HDRSRC)
	gcc -o $(HDROBJ) -c $(HDRSRC)

clean:
	rm -f $(ESEG)
	rm -f *.o
