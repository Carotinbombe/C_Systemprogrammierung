#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/param.h>
#include <fcntl.h> 
#include <linux/limits.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

char *isSoftlink(char *path){
	
	static char data [PATH_MAX+1];
	struct stat buf;
	
	/*Prüfe ob Datei in Path existiert*/
	if(lstat(path,&buf)<0) {
            perror("lstat"); return NULL;}
	
	/*Datei ist Softlink*/
	if((buf.st_mode &S_IFMT)==S_IFLNK){
		/*Schreibe Ergebnis in byte*/
		ssize_t byte = readlink(path,data,PATH_MAX);
		/*Setze Endezeichen am Ende des Strings*/
		data[byte]='\0';
		/*Gebe data zurück*/
		return data;
		}
	
	return NULL;
}