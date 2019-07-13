#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>


int show(struct stat *status);

int main(int argc, char **argv)
{
	/*Strukturen und Variablen*/
	struct stat status;
	struct dirent *directory;
	DIR *dir;
	int input;

	/*Fehler, wenn nichts übergeben*/
	if(argc < 2){
		printf("Bitte Parameter übergeben!\n");
		return EXIT_FAILURE;}

	/*Speichere Zustand von stat in Input*/
	input = stat(argv[1],&status);
	
	/*Fehler, wenn stat -1*/
	if(input < 0){
		perror("Fehler bei Statusabfrage!\n");
		return EXIT_FAILURE;}
	
	/*Gebe übergebenes Element aus*/
	input=show(&status);
	if(input<0) return EXIT_FAILURE;
	
	/*Wenn Objekt directory, öffne es*/
	if(status.st_mode & S_IFDIR){
		/*Öffnen des Verzeichnises*/
		if((dir=opendir(argv[1])) == NULL)
		{perror("Fehler bei Abfrage Verzeichnis!\n");
			return EXIT_FAILURE;}
		
		/*Wechesln in das Verzeichnis*/
		if ((chdir(argv[1])) < 0)
		{perror("Fehler bei wechseln des Verzeichnis!\n");
			return EXIT_FAILURE;}
		
		/*Gebe alle Elemente im directory aus*/
		while((directory=readdir(dir))!= NULL){
			/*Aktuelle Dateinamen ausgeben*/
			printf("Aktuelle Pos.: %s\n\n",directory->d_name);
			
			/*stat in input Speicher für evtl. Fehler*/
			input=stat(directory->d_name,&status);
			
			/*Wenn Fehler, dann Ausgabe*/
			if(input < 0)
			{perror("Fehler bei Statusabfrage!\n");
				return EXIT_FAILURE;}
			
			/*Anzeigen*/
			input=show(&status);
			if(input<0) return EXIT_FAILURE;
			}}
	/*Schliessen des directory*/
	input=closedir(dir);
	if (input < 0){
		perror("closedir() ");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int show (struct stat *status)
{
	struct passwd *user;
	struct group *grp;


	char rwx[] = "rwxrwxrwx";
	int bits[] = {
		S_IRUSR,S_IWUSR,S_IXUSR,   /* Zugriffsrechte User    */
		S_IRGRP,S_IWGRP,S_IXGRP,   /* Zugriffsrechte Gruppe  */
		S_IROTH,S_IWOTH,S_IXOTH    /* Zugriffsrechte der Rest */}; 

	int typ[]= {S_IFREG,S_IFDIR,S_IFCHR}; 

	char *typC[]= {"Reguläre Datei", "Verzeichnis", "Gerätedatei"};

	user = getpwuid(status->st_uid);
	grp = getgrgid(status->st_gid);

	if(user < 0)
	{
		perror("Fehler bei Namensabfrage!\n");
		return EXIT_FAILURE;
	}
	if(grp < 0)
	{
		perror("Fehler bei Gruppenabfrage!\n");
		return EXIT_FAILURE;
	}
	printf("------ Informationen -----\n\n");
	printf("Geraetenummer: \t\t\t\t%lu\n",status->st_dev);
	printf("i-Node Nummer: \t\t\t\t%lu\n",status->st_ino);
	printf("Linkzaehler: \t\t\t\t%lu\n",status->st_nlink);

	for(int i = 0 ; i < 2;i++)
	{
		if(status->st_mode & typ[i]){
			printf("Dateityp: \t\t\t\t%s\n",typC[i]);
		}
	}

	printf("Zugriffrechte: \t\t\t\t");

	for(int i = 0; i < 9; i++) {

		if(status->st_mode & bits[i])
		printf("%c",rwx[i]);  /*r,w oder x*/
		else
		printf("-"); /*wenn nicht gesetzt, dann '-'*/
	}
	printf("\n");
	printf("Besitzer: \t\t\t\t%s : %u\n",user->pw_name,status->st_uid);
	printf("Gruppe: \t\t\t\t%s : %u\n",grp->gr_name,status->st_gid);
	printf("Dateigroesse:\t\t\t\t%lu\n",status->st_size);
	printf("Letzter Zugriff:\t\t\t%s\n",ctime(&status->st_atime));
	printf("Letzte Modifikation:\t\t\t%s\n",ctime(&status->st_mtime));
	printf("Letzte Statusaenderung:\t\t\t%s\n",ctime(&status->st_ctime));
	return 0;
}