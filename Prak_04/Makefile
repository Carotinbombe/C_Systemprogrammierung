all: Prak04_12 Prak04_3

Prak04_12: Prak04_12.o bubble.o bubble.h
	gcc -g -std=c99 -Wall -o Prak04_12 Prak04_12.o bubble.o 

Prak04_3: Prak04_3.o bubble.o bubble.h
	gcc -g -std=c99 -Wall -o Prak04_3 Prak04_3.o bubble.o

Prak04_12.o : Prak04_12.c
	gcc -g -std=c99 -Wall -c Prak04_12.c

bubble.o: bubble.c bubble.h
	gcc -g -std=c99 -Wall -c bubble.c

Prak04_3.o: Prak04_3.c
	gcc -g -std=c99 -Wall -c Prak04_3.c
	
clean: 
	rm *.o  Prak04_12 Prak04_3

