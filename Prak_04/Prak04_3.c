#include <stdio.h>
#include <stdlib.h>
#include "bubble.h"
/* Autor: Robin Gerstmann*/

/*Funktion deklarieren*/
void zeigeFeld(char ***ptr, int anzahl);

/*  Main   */
int main(int argc, char **argv){

    
    /*Pruefe ob Eingabe mind. 1 Zeichen uebergibt*/
    if (argc < 2) {fprintf(stderr, "Bitte Parameter uebergeben!\n");return EXIT_FAILURE;}
    /*Berechne Anzahl der uebergeben Parameter*/
    int anzahl=argc -1;	
    char ***ptr=0;

    /*Anzahl Parameter ausgeben*/
    printf("Anzahl der Parameter: %i\n", anzahl);
    
    /*Speicherallokierung für char pointer*/
    ptr=malloc(anzahl * sizeof(char*));
    
    if(ptr==NULL){
        fprintf(stderr, "Fehler bei Speicherallokation\n");return EXIT_FAILURE;
    }


    /*Speicher uebergebene Parameter*/
    for (int i = 0 ; i < anzahl ;i++){
        ptr[i]=&argv[i+1];}
    
    /*Sortiere Stings*/
    bubbleString(ptr,anzahl);

    /*Gebe Werte in Konsole aus*/
    zeigeFeld(ptr,anzahl);
    
    /*Speicher wieder freigeben*/
    free(ptr);
    
    /*Return 0 = EXIT_SUCCESS*/
    return 0;
}

/*  Funktion zur Ausgabe des Feldes*/
void zeigeFeld(char ***ptr, int anzahl){
    /*Werte von Stringfeld durch dereferenzieren ausgeben*/
    for (int i = 0 ; i < anzahl ;i++){
        printf("Buchstabe von Feld  %d :\t %s\n",i+1,*ptr[i]);}
}

