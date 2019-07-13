#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <stddef.h>
#include <signal.h>
#include "settings.h"


int fd_src;
int socketfd;

void exitHandler(){
  
    if(close(fd_src) < 0){
            perror("close()");
            _exit(EXIT_FAILURE);}
    
    printf("closed Filedescriptor\n");
    
    if(close(socketfd) < 0){
            perror("close()");
            _exit(EXIT_FAILURE);}
    printf("closed socket filedescriptor\n");
    
    
    
    
}

int main (int argc, char *argv[]){
    
  
    char buffer[BUFFERSIZE];
    struct sockaddr_un adress;
    struct SocketMessage msg_client;
    int erg_r;
	int addrlen = 128;
  
    
    
    /*Parameter prüfen*/
    if (argc < 3){
        fprintf(stderr,"Bitte Parameter übergeben [QUELLE] [ZIEL]!\n");
        exit(EXIT_FAILURE);}
    
    /*Öffne Datei*/
    if((fd_src = open(argv[1],O_RDONLY)) < 0){
        perror("fd_src");
        exit(EXIT_FAILURE);}
    
    /*Aktiviere exitHandler*/
    atexit(exitHandler);
   
    /*Socket erstellen*/ 
    if((socketfd = socket(PF_LOCAL, SOCK_STREAM,0)) < 0){
        perror("socket()");
        exit(EXIT_FAILURE);}
   
   /*Struct vorbereiten*/    
    adress.sun_family = AF_LOCAL;  
    strncpy(adress.sun_path,  SOCKET_PATH, sizeof(adress.sun_path)-1);
    addrlen = strlen(adress.sun_path) + offsetof(struct sockaddr_un, sun_path);
        
    
    /*Socket connect*/
    if(connect(socketfd, (struct sockaddr*) &adress, addrlen) < 0){
		perror("connect()");
        exit(EXIT_FAILURE);}
    
	/*Sende PATH*/
	msg_client.msg_typ=DATA_PATH;
	strncpy(msg_client.msg_text,argv[2],sizeof(argv[2]));
	msg_client.msg_size=sizeof(argv[2]);
	
	if(send(socketfd, &msg_client,sizeof(msg_client),0) < 0){
		perror("Message send failure");
		exit(EXIT_FAILURE);}
    
	
	while((erg_r = read(fd_src,buffer,BUFFERSIZE)) != 0)
		{
            /*Wenn Fehler bei read*/
			if(erg_r <0 ){
				perror("read()");
				exit(EXIT_FAILURE);}
			/*Schreibe in den Buffer*/
			strncpy(msg_client.msg_text, buffer,erg_r);
			/*Gelesene Bytes in Client-Package*/
			msg_client.msg_size=erg_r;
			msg_client.msg_typ=DATA_SEND;
			
			/*Sende Message-Package*/
			if(send(socketfd, &msg_client,sizeof(msg_client),0) < 0){
				perror("Message send failure");
				exit(EXIT_FAILURE);}
			printf("Package sended\n");
			
			/*Warte auf Anwtort von Server*/
			if(recv(socketfd,&msg_client,sizeof(msg_client),0) < 0){
				perror("Message receive error");
				exit(EXIT_FAILURE);}
			
			if(msg_client.msg_typ==DATA_SUCC){
				printf("received ACK\n");}
			
			/*Falls Server Fehler meldet*/
			if(msg_client.msg_typ==DATA_FAIL){
				printf("Error while sending ");
				exit(EXIT_FAILURE);}}
	msg_client.msg_typ=DATA_SUCC;	
	if(send(socketfd, &msg_client,sizeof(msg_client),0) < 0){
		perror("Message send failure ");
		exit(EXIT_FAILURE);}
			
    exit(EXIT_SUCCESS);

}
