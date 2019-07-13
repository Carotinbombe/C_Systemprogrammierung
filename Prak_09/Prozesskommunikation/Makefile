all: client server

client: client.o
	gcc -g -Wall -o client client.o

client.o: client.c
	gcc -g -Wall -c client.c

server: server.o
	gcc -g -Wall -o server server.o

server.o: server.c
	gcc -g -Wall -c server.c

clean:
	rm *.o client server
	clear
