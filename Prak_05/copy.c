#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>



int main(int argc, char **argv)
{
    struct stat status;
    int fd_src;
	int fd_dest;
    int erg_r;
	int erg_w;
    char *buffer;
    int sum;
    int R_BYTE;
    
    R_BYTE = atoi(argv[3]);
    
    if(isdigit(argv[3] && argc > 3)){R_BYTE=512;}
    
    if(argc < 3){
        fprintf(stderr, "Fehlerhafte Angaben!\nErwartet wird: ./Prak05 [QUELLE] [ZIEL] [BUFFERSIZE] \n\n");
        return EXIT_FAILURE; 
    }
    
    /*Filedeskriptor*/
    fd_src = open(argv[1],O_RDONLY);
	fd_dest = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT);
    
    /*Pruefe*/
    if(fd_src < 0 || fd_dest < 0){
        perror("Filedeskriptorfehler");
        return EXIT_FAILURE;
    }
    //ACCESS datei existiert bereits?!????
    erg_r=stat(argv[1],&status);
    
    if(erg_r < 0){
        perror("FEHLER");
        return EXIT_FAILURE;
    }
    
    buffer = malloc(R_BYTE*sizeof(char));
    
    if (buffer < 0){
        perror("FEHLER");
        return EXIT_FAILURE;
    }
    printf("---------- Copy ---------\n\n");
    printf("Buffer wurde auf %i Byte festgelegt.\n",R_BYTE);
    printf("Dateigroesse: \t%li Byte\n",status.st_size);
    printf("Datei wird von >%s< nach >%s< kopiert.\n",argv[1],argv[2]);
    printf("Rechte der Datei: ");

	
	printf("Datei wird kopiert... \n");
	while((erg_r = read(fd_src,buffer,R_BYTE)) != 0)
	{
	if(erg_r <0 )
	{
		perror("Fehler beim Lesen der Datei");
        return EXIT_FAILURE;    
	}
	erg_w= write(fd_dest,buffer,R_BYTE);
    
	if(erg_w <0 )
	{
		perror("Fehler beim Schreiben der Datei");
        return EXIT_FAILURE;    
	}
    }
	
    free(buffer);
    
    return EXIT_SUCCESS;
}