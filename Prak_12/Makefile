all: client server

client: client.o
	gcc -g -Wall -o client client.o

client.o: client.c settings.h
	gcc -g -Wall -c client.c

server: server.o
	gcc -g -Wall -o server server.o -pthread
	rm *.o

server.o: server.c settings.h
	gcc -g -Wall -c server.c -pthread

clean:
	rm *.o client server
	clear
