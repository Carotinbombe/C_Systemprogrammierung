prog: kreis.o flaeche.o flaeche.h
	gcc -g -o kreis kreis.o flaeche.o -lm

kreis.o: kreis.c
	gcc -g -Wall -c kreis.c

flaeche.o: flaeche.c
	gcc -g -Wall -c flaeche.c

clean:
	rm *.o kreis