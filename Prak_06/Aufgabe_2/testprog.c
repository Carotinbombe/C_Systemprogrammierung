#include "isdir.c"
#include <stdio.h>


int main(int argc, char **argv)
{
	printf("----------Welcome----------\n");
	/*Führe Funktion aus und Unterscheide*/
	switch(isdir(argv[1])){
		case 0:
			printf("%s ist ein Verzeichnis.\n",argv[1]);
			break;
		case 1:
			printf("%s ist kein Verzeichnis.\n",argv[1]);}
	return 0;
}
