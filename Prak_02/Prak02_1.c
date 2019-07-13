#include <stdio.h>
#include <stdlib.h>

int main(){
	/*Eingabevariable*/
	char x;

	/*Wiederhole solange bis q eingegeben wurde*/
	do{
		printf("Eingabe ('q' quit): ");
		/*Eingabe an Terminal*/
		scanf(" %c",&x);
                /*getChar fuer einlesen eines Zeichnes aucgh bei Zeichenkette -> funktioniert nur bedingt(!)*/
		getchar();
		/*Wenn x == q dann beende das Programm*/
		if(x=='q') 
			return EXIT_SUCCESS;
		/*Ausgabe des eingegeben Zeichen*/
		printf("Ausgabe: '%c'  %c  %u  %x\n",x,x,x,x);
		}while(1);

	
}
