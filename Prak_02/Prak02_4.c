#include <stdio.h>
#include <stdlib.h>

/*Erstelle einer Struktur namens Test*/
	struct test {int x;
		char c;
		
	};

	/*Erstellen einer Union*/
	union Data{
		struct test st;
		unsigned char c [8];
	};
	
int main(){

	/*Variable der Struktur und Union instanzieren*/
	struct test s_test;
	union Data u_data;
	/*Werte festlegen*/
	s_test.c= 'z';
	s_test.x=100000;

	u_data.st.c='z';
	u_data.st.x=1000000;
	
	printf("\n---------Allgemeine Daten--------------------\n");
	
	/*Ausgabe der Werte*/
	printf("Struktur  Char:  \t%c \n",s_test.c);
	printf("Struct Integer:  \t%u \n",s_test.x);
	/*Struct- und Uniongroesse ausgeben*/
	printf("Groesse: Struct  \t%lu \n",sizeof(s_test));
	printf("Groesse: Union   \t%lu \n",sizeof(u_data));
	
	/* %p pointer mit referenz auf test1/2*/
	printf("Union Adresse:   \t%p \n",&s_test);
	printf("Struktur Adresse:\t%p \n",&u_data);
	printf("\n---------Ausgabe der Werte von Union---------\n");
	//Schleife für Ausgabe der Daten von u_data
	for(int i = 0; i <= 7; i++) 
		{printf("Index %d:\t0x%02x\tAdresse: %p\n",i ,u_data.c[i], &(u_data.c[i]));}
	
	return EXIT_SUCCESS;
}

