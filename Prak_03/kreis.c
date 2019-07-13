/* ----------------------- Berechnungen am Kreis ------------------------ */

/* Definition der Konstanten Pi durch Präprozessoranweisung #define       */
/* jedes PI wird im Text durch 3.1415926 ersetzt                          */


/* Einfügen benötigter Headerdateien in Quelltext   */
/* - enthalten Prototypen von Bibliotheksfunktionen */
/*   wie printf(), scanf()                          */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "flaeche.h"


/*Pi als Definition -> Assembler wie equ Anweisung*/
#define PI 3.1415926

/* Main Funktion               */
int main(int argc, char *argv[])
{
    /* Definition lokaler Variablen                 */
    double r, u, flaeche;
    char jn;
    int erg;
    
    /* do-while Schleife für Wiederholen, sollte User 'j' eingeben */
    do {
        printf("Geben Sie den Radius ein : ");
        
		/*Eingabe von Radius*/
		erg=scanf("%lf",&r);     
		
		/*Sollte Buchstabe eingegeben werden, ist erg=0 -> Fehler!*/
        if(erg==0)
			{fprintf(stderr, "Eingabefehler!\n");return EXIT_FAILURE;}
		
		/*Berechne Umfang*/
		u = 2*PI*r;
        
        /*Berechne Kreisflaeche*/
        flaeche = kreisFlaeche(r);
        
		/*Ausgabe von Umfang und Flaeche für den eingegeben Radius*/
        printf("Umfang: %lg  Flaeche: %lg \n", u, flaeche);
        printf("Nochmal (j/n): ");
		
		/*Abfrage nochmal?*/
        erg = scanf(" %c", &jn);
		/*Sollte Eingabe Fehlerhaft sein -> wird Programm abgebrochen*/
		if(!(jn=='j'||jn=='n'))
			{fprintf(stderr, "Eingabefehler!\n");return EXIT_FAILURE;}
		
    } while(jn == 'j');
    return EXIT_SUCCESS;     /* EXIT_SUCCESS (0) alles ok */
}
