.DEFAULT_TARGET := all

all:  mylib test

mylib: isdir
	ar r ../libmylib.a isdir.o

isdir: isdir.c isdir.h
	gcc  -Wall -g -c -o isdir.o isdir.c

test: testprog.c mylib
	gcc  -Wall -g  -o testprog testprog.c -L../ -lmylib
	rm *.o 
clean:
	rm testprog 