#include <errno.h>
#include <stdio.h>
#include <unistd.h>  
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>


/*Gibt 0 zurück, wenn path ein Verzeichnis ist. Sonst -1*/
int isdir(char *path){
	int input;
	struct stat status;
	
	input=stat(path,&status);
	/*Fehlerbehandlung*/
	if(input < 0){
		 perror("Fehler bei Statusabfrage!\n");
         return EXIT_FAILURE;
	}
	/*Gebe 0 zurück, wenn DIRECOTRY, sonst -1*/
	if(status.st_mode & S_IFDIR) return 0;    
	
	return 1;
}