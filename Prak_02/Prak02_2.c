#include <stdio.h>
#include <stdlib.h>

int main(){
	/*Variable zur Eingabe des Zeichens*/
	char x;

	/*Wiederhole solange bis q eingegeben wird*/
	do{
		printf("Eingabe ('q' quit):");
		/*Einlesen Eingabe in x*/
		scanf(" %c",&x);
		/*Wenn x == q dann schliesse Programm*/
		if(x=='q') 
			return EXIT_SUCCESS;
		/*Gebe Eingabe in c aus mit Feldbreite 4 und als Char interepretieren*/
		printf("Eingabe:\t'%4c'\n",x);
		printf("Char:   \t %4c \n",x);
		/*Ausgabe in Integer mit 0 auffuellen und 3 Stellen*/
		printf("Integer:\t  %03u\n",x);
		/*Ausgabe in Hexadezimal z.B 0x00 mit Feldbreite 4*/
		printf("Hexadez:\t %#4x\n\n",x);
		}while(1);

	
}
