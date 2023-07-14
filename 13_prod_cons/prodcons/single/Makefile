ESEG = prod

LIBNAME = semaphore
LIBOBJ = $(LIBNAME).o
LIBSRC = $(LIBNAME).c
LIBHDR = $(LIBNAME).h

SRCNAME = main
SRCOBJ = $(SRCNAME).o
SRCSOURCE = $(SRCNAME).c

all: $(ESEG)

$(ESEG): $(LIBOBJ) $(SRCOBJ)
	gcc -o $(ESEG) $(LIBOBJ) $(SRCOBJ)

$(LIBOBJ): $(LIBHDR) $(LIBSRC)
	gcc -o $(LIBOBJ) -c $(LIBSRC)

$(SRCOBJ): $(SRCSOURCE) $(LIBHDR)
	gcc -o $(SRCOBJ) -c $(SRCSOURCE)

clean:
	rm -f *.o
	rm -f $(ESEG)
