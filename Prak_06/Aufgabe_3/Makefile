SoftlinkTester: SoftlinkTester.o isSoftlink.o
	gcc -g -Wall -o SoftlinkTester SoftlinkTester.o isSoftlink.o

isSoftlink.o: isSoftlink.c isSoftlink.h
	gcc -g -Wall -c isSoftlink.c

SoftlinkTester.o: SoftlinkTester.c
	gcc -g -Wall -c SoftlinkTester.c


addLib: isSoftlink.o 
	ar r ../libmylib.a isSoftlink.o

clean:
	rm -f *.o
	rm SoftlinkTester