DIRS = monitor codice

all: subs

subs:
	for dir in $(DIRS) ; do make -C $$dir ; done

clean:
	for dir in $(DIRS) ; do make clean -C $$dir ; done
