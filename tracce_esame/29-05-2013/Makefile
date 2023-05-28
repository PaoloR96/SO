ESEG = main client server
FLG = -pthread

all: $(ESEG)

main: main.o header.o
	gcc $(FLG) -o $@ $^

client: client.o header.o
	gcc $(FLG) -o $@ $^

server: server.o header.o
	gcc $(FLG) -o $@ $^
	
header.o: header.c header.h
	gcc -o $@ -c $<
	
clean:
	rm -f *.o
	rm -f $(ESEG)

