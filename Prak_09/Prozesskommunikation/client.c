#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


#define BUFFERSIZE 1024

#define MSG_SERV 100
#define MSG_CLNT 99
#define DATA_PATH 1
#define DATA_SUCC 2
#define DATA_FAIL -1
#define DATA_SEND 3

int fd_src;

void exitHandler(){
  
    if(close(fd_src) < 0){
            perror("close()");
            _exit(EXIT_FAILURE);}
    printf("\nclosed Filedescriptor\n");
    
}

int main (int argc, char *argv[]){
    
    
    char buffer[BUFFERSIZE];
    int erg_r;
    char *serverkey = "./server";
    key_t key;
    int msqid;
    /*Struct für Message Queue*/
    struct msgbuffer{
        long mtype;
        char mtext[BUFFERSIZE];
        ssize_t size;
        long datatype;
    }msg_client,msg_server;
    
    /*Parameter prüfen*/
    if (argc < 3){
        printf("Bitte Parameter übergeben [QUELLE] [ZIEL]!\n");
        return EXIT_FAILURE;}
    /*Öffne Datei*/
    if((fd_src = open(argv[1],O_RDONLY)) < 0){
        perror("fd_src");
        return EXIT_FAILURE;}
    /*Aktiviere exitHandler*/
    atexit(exitHandler);
    
    /*Key generieren*/
    if((key = ftok(serverkey,1)) < 0){
        perror("ftok()");
        exit(EXIT_FAILURE);}
    
    /*Anlegen einer Message Queue*/
    printf("waiting for connection (quit with ^C)\n");

	
    /*Warte auf Verbindung*/
    while((msqid=msgget(key, 06000) )< 0){
        sleep(1);
        printf(".");
        fflush(0);}
    
	/*Struct vorbereiten*/
    msg_client.mtype = MSG_CLNT;
    msg_client.datatype = DATA_PATH;
    strcpy(msg_client.mtext,argv[2]);
	
    printf("\nServer connected\n");
   
   /*Sende Path*/
	if(msgsnd(msqid,&msg_client,sizeof(msg_client),0) < 0){
        perror("Message send failure");
         exit(EXIT_FAILURE);}
	
	/*Warte auf Anwtort*/
	printf("Waiting for answer\n");
	
	/*Empfange Nachricht -> Fehlerausgabe*/
	if(msgrcv(msqid,&msg_server,sizeof(msg_server),MSG_SERV,0) < 0){
		perror("Message receive error\n");
		exit(EXIT_FAILURE);}
	
	/*Wenn Datei schon vorhanden ist*/
	if(msg_server.datatype==DATA_FAIL){
		printf("%s\n",msg_server.mtext);
		exit(EXIT_FAILURE);}
	/*War Server erfolgreich*/
	else if(msg_server.datatype==DATA_SUCC){
		printf("Sending data\n");
		/*Setze Datatyp zu DATA_SEND zum senden der Datei*/
		msg_client.datatype=DATA_SEND;
		
		/*Lese Datei nach un nach mit BUFFERSIZE bytes speichere es in buffer
		öffne einen Filedeskriptor*/
		while((erg_r = read(fd_src,buffer,BUFFERSIZE)) != 0)
		{
			/*Wenn Fehler bei read*/
			if(erg_r <0 ){
				perror("read()");
				exit(EXIT_FAILURE);}
			/*Schreibe in den Buffer*/
			strncpy(msg_client.mtext, buffer,erg_r);
			/*Gelesene Bytes in Client-Package*/
			msg_client.size=erg_r;
			
			printf("Send ack\n");
			/*Sende ACK*/
			if(msgsnd(msqid,&msg_client,sizeof(msg_client),0) < 0){
				perror("Message send failure");
				exit(EXIT_FAILURE);}
			/*Warte auf Anwtort von Server*/
			if(msgrcv(msqid,&msg_server,sizeof(msg_server),MSG_SERV,0) < 0){
				perror("Message receive error");
				exit(EXIT_FAILURE);}
			
			printf("received ack\n");
			/*Falls Server Fehler meldet*/
			if(msg_server.mtype==DATA_FAIL){
				printf("Error while sending");
				exit(EXIT_FAILURE);}}
			}
	/*Schliesse Server mit DATA_SUCC*/
	msg_client.datatype=DATA_SUCC;
	if(msgsnd(msqid,&msg_client,sizeof(msg_client),0) < 0){
		perror("Message send failure");
		exit(EXIT_FAILURE);}
	printf("file was successfully copied\n");	
    
    exit(EXIT_SUCCESS);
}