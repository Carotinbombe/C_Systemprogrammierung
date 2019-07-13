#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "isSoftlink.h"
 
int main(int argc, char* argv[]) {
	/*Fehler bei keinen Parameter*/
	if (argc < 2){
		printf("Argument [PATH] erwartet!\n");
		return EXIT_FAILURE;
	}
	/*Rufe ab - Unterscheide ob Softlink oder nihct*/
	char *data=isSoftlink(argv[1]);
	
	if(data == NULL){
		printf("Die Datei %s ist kein Softlink\n",argv[1]);
	}
	
	else{
		printf("Die Datei %s ist ein Softlink\n",argv[1]);
		printf("Pfad: %s \n",data);
	}
		return EXIT_SUCCESS;
}

/* 	--------Antworten---------- */
/* 	Warum Static? */
/* 	Die Funktion soll einen Zeiger auf eine Zeichenkette mit dem Namen der Datei,
	auf die der Softlink zeigt.
	Ohne Static geht dieser Zeiger bzw der Inhalt verloren*/
  
/*	Der Zeiger wird überschrieben */

/*	Lösung: Neue Zeigervariable anlegen, jedoch mehr Verwaltungsaufwand!*/