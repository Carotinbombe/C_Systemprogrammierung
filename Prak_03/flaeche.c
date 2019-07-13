#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "flaeche.h"

static double PI();     //static macht, dass PI außerhalb der flaeche.c nicht sichtbar 
/*Variable wird nur einmal angelegt*/
volatile int counter = 0;
/*Statische Methode nur innerhalb der Datei aufrufbar*/
static double PI()
{
    return (2*asin(1));
}
/*Gibt double Wert von Kreisflache zurück
 bei groeßer 5 gibt es eine Warnung*/
double kreisFlaeche(double radius)
{
    counter++;
    if(counter >= 5)
    {
         printf("!!Achtung Radius wurde bereits %dx aufgerufen!!\n", counter);
    }
    
    return (PI()*radius*radius);
}

//Zusatzaufgabe 4 Der Prototyp muss nicht aufgenommen werden, da die Funktion nur in der flaeche.c verwendet wird.
//Wenn die Funktion in weiteren Datei verwendet werden würde, wäre es hingegen notwendig