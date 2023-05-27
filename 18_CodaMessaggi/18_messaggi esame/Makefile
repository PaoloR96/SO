all: start client server

start: main.c client server lib.h
	gcc -pthread -o start main.c
	
client: client.c lib.h 
	gcc -o client client.c
	
server: server.c lib.h
	gcc  -pthread -o server server.c
	
clean:
	rm -f *.o
	rm -f start client server