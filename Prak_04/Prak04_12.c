#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bubble.h"

/* Autor: Robin Gerstmann*/
void zeigeFeld(int **ptr, int anzahl);

/*  Main   */
int main(int argc, char *argv[]){
/*  Initalisiere Pointer       */
    int *feld=0;
    int **ptr=0;
    
    
    
    /*Bei falscher Eingabe*/ 
    if ((argc < 2 || !isdigit(*argv[1])))
        {fprintf(stderr, "Eingabefehler!\n");return EXIT_FAILURE;}
   
   /* Uebergebene Zahl speichern*/
    int anzahl=atoi(argv[1]);
    
    /*Dynamisches Feld für Zeiger und Integer anlegen*/
    feld= malloc(anzahl * sizeof(int));
    ptr=malloc(anzahl * sizeof(int*));
    
    if(feld==NULL || ptr == NULL){
        fprintf(stderr, "Fehler bei Speicherallokation\n");return EXIT_FAILURE;
    }

    /*Feldern mit Werten zuweisen*/
    for (int i = 0 ; i < anzahl ;i++){
        feld[i]=rand();}

    /*Zeigefeld mit Referenz auf Feld*/
    for (int i = 0 ; i < anzahl ;i++){
        ptr[i]=&feld[i];}

    printf("Feldlaenge: %d\n",anzahl);

    /*Sortieren*/
    bubble(ptr,anzahl);
    /*Anzeigen des Feldes*/
    zeigeFeld(ptr,anzahl);

    /*Speicher freigeben*/
    free(feld);
    free(ptr);
    return 0;
}

/*  Funktion zur Ausgabe des Feldes*/
void zeigeFeld(int **ptr, int anzahl){
    /*Werte von Feldern ausgeben*/
    for (int i = 0 ; i < anzahl ;i++){
        printf("Wert von Feld  %d :\t %d\n",i+1,*ptr[i]);}
}

/*Hilffunktion zur Ausgabe der Adressen von ptr*/
void zeigeAdresse(int **ptr, int anzahl){
    /*Werte von Feldern ausgeben*/
    for (int i = 0 ; i < anzahl ;i++){
        printf("Adresse:  %d : %p\n",i+1,ptr[i]);}
}

