/* Sortieralgorithmus Bubble Sort */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bubble.h"

/*Einfacher Sortieralgorithmus*/
void bubble(int **ptr,int anzahl){
    /*Temporärer zeiger*/
    int *temp=0;

    /*Sortieren des Arrays an Zeigern*/
    while(anzahl--)
    {
        /*Betrachte Wert von Pointer i und Pointer i+1*/
        for (int i = 1; i <=anzahl;i++){

            /*Ist Wert von Pointer i-1 größer als von Pointer i*/
            if(*ptr[i-1] > *ptr [i]){

                /*Sichere Pointe von i*/
                temp=ptr[i];
                
                /*Überschreibe Pointer von i mit Pointer i - 1*/
                ptr[i]=ptr[i-1];
                /*Speichere in Pointer von i-1 den gesicherten Pointer*/
                ptr[i-1]=temp;
            }
        }
    }
}
/*Sortieralgorithmus für Strings*/
void bubbleString(char ***ptr,int anzahl)
{
    /*Temporäre Zeiger*/
    char **temp=0;

    /*Sortieren*/
    while(anzahl--)
    {
        for (int i = 0; i <anzahl;i++){
            /*>0 das erste ungleiche Zeichen in str1 ist größer als in str2*/
            if(strcmp((const char*)*ptr[i],(const char*)*ptr[i+1]) > 0){

                /*Sichere Pointer an Stelle i*/
                temp=ptr[i];
                
                /*Tausche Pointer */
                ptr[i]=ptr[i+1];
                /*Gesicherter Pointer in Ptr+1*/
                ptr[i+1]=temp;
            }
        
            
        }
    }
}


/*
0 die Strings sind gleich
>0 das erste ungleiche Zeichen in str1 ist größer als in str2
<0 das erste ungleiche Zeichen in str1 ist kleiner als in str2
*/