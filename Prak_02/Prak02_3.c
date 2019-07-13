#include <stdio.h>
#include <stdlib.h>

int main(){
	/*Ausgabe von Byte für Datentyp mit sizeof(DATA_TYPE) Methode*/
	/* \t lässt ein tab frei und %u fuer unsigned integer als Wert von sizeof()*/
	printf("Der Typ char belegt \t %lu  Adressen\n",sizeof(char));
	printf("Der Typ short belegt \t %lu  Adressen\n",sizeof(short));
	printf("Der Typ int belegt \t %lu  Adressen\n",sizeof(int));
	printf("Der Typ long belegt \t %lu  Adressen\n",sizeof(long));
	printf("Der Typ double belegt \t %lu  Adressen\n",sizeof(double));
	printf("Der Typ float belegt \t %lu  Adressen\n",sizeof(float));
	/*Alles okay -> return 0*/
	return EXIT_SUCCESS;
}

