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
int fd_sock;
enum mechanism{true,false};

/*Exithandler schliesst Socket und Filedeskriptor*/
void exitHandler(){
   printf("-----EXIT-HANDLER-----\n\n");
   if(unlink(SOCKET_PATH) < 0 ){
        perror("unlink()");
        _exit(EXIT_FAILURE);}
    printf("unlinked socket\n");
	
	if(close(fd_src) < 0){
            perror("close()");
            _exit(EXIT_FAILURE);}
    printf("closed Filedescriptor\n");
	
	if(close(fd_sock) < 0){
		perror("close()");
        _exit(EXIT_FAILURE);}
    printf("closed socket fd\n");
	
	
	
}
/*Interrupt durch ^C ruft Exithandler auf*/
void Interrupt(){
    exitHandler();
    _exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]){
	
	/*Socket*/
	struct sockaddr_un server_addr;
	int addrlen = 128;
	int commsockfd;
       
	
        /*Enum*/
	enum mechanism isLocked;
	/*Message*/
	struct SocketMessage server;
	
	/*Signalhandling*/
	struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler=Interrupt;
	
 
	 /*Signal erzeugen ^C*/
    if(sigaction(SIGINT,&act,NULL) < 0){
		perror("sigaction()");
		return EXIT_FAILURE;}
    printf("Siganction activated\n");  
	
	/*Aktiviere exitHandler*/
    atexit(exitHandler);
    printf("Exit handler activated\n");
 
	/*Lege Socket an*/
	if((fd_sock = socket(PF_LOCAL, SOCK_STREAM,0)) < 0){
		perror("socket()");
        return EXIT_FAILURE;}
	printf("Socket activated\n");	
    
	/*Socket vorbereiten*/
	server_addr.sun_family = AF_LOCAL;  
        strncpy(server_addr.sun_path,  SOCKET_PATH, sizeof(server_addr.sun_path)-1);
        addrlen = strlen(server_addr.sun_path) + offsetof(struct sockaddr_un, sun_path);
   

    /* Adresse binden */
    if(bind(fd_sock, (struct sockaddr *)&server_addr, addrlen) < 0){
        perror("bind()");
        exit(EXIT_FAILURE);}

	/* Jeder Nutzer soll diesen Socket verwenden können        */
    if(chmod(SOCKET_PATH, 0777) < 0){
        perror("chmod()");
        exit(EXIT_FAILURE);}


	/* Warteschlange initialisieren */
    if(listen(fd_sock, 5) < 0) {
        perror("listen()");
        exit(EXIT_FAILURE);}
    
    isLocked=false;
	while(1){
		/* auf Auftrag warten */
            if(isLocked == false){
                printf("Enum locked\n");
                if((commsockfd = accept(fd_sock, NULL, NULL)) < 0){
                    perror("accept");
                    exit(EXIT_FAILURE);}
                isLocked=true;}
			  
            if(recv(commsockfd, &server, sizeof(server), 0) < 0){
                perror("recv()");
                exit(EXIT_FAILURE);}
		
		printf("Received data:\n");
		printf("MSG_TYPE: %u\n",server.msg_typ);
		printf("MSG_SIZE: %lu\n",server.msg_size);
		printf("MSG_TEXT: %s\n",server.msg_text);
		
		
		switch(server.msg_typ){
			/*Pfad empfangen*/
			case DATA_PATH:	
				printf("copy to path: %s\n",server.msg_text);
				/*Erstelle Datei-> Fehler wenn Datei vorhanden*/
				if((fd_src=open(server.msg_text, O_RDWR | O_CREAT | O_EXCL)) < 0){
					perror("fehler");
					
					/*Struct vorbereiten*/
					server.msg_typ=DATA_FAIL;
					
					/*Wenn Datei existiert*/
					if(errno == EEXIST){strcpy(server.msg_text,"file already exist\n");}
					
					/*Wenn Datei nicht anleget oder andere Fehler auftreten */
					else {strcpy(server.msg_text,"error with open stream!\n");}
					server.msg_size=sizeof(server.msg_text);
					
					/*Gebe Client Bescheid*/
					if(send(commsockfd, &server,sizeof(server),0) < 0){
						perror("Message send failure");}
						exit(EXIT_FAILURE);}
				
				/*Datei angelegt und Client ACK gesendent*/
				else{
					printf("File created\nSending ACK\n");
					server.msg_typ=DATA_SUCC;
                                        if(send(commsockfd, &server,sizeof(server),0) < 0){
						perror("Message send failure");
						exit(EXIT_FAILURE);}
                                        printf("sended ACK\n");}
				
					
				break;
			/*Empfange Daten*/
			case DATA_SEND:
				/*Schreibe in Datei*/
				if (write(fd_src,server.msg_text,server.msg_size) < 0){
					perror("write()");
                                        exit(EXIT_FAILURE);}
				
				printf("Data write successfully\n");
				
				/*ACK zum Client*/
				server.msg_typ=DATA_SUCC;
				if(send(commsockfd, &server,sizeof(server),0) < 0){
						perror("Message send failure");
						exit(EXIT_FAILURE);}
				break;
			
			/*Schliesse Filedeskriptor*/
			case DATA_SUCC:
				if(close(fd_src) < 0){
					perror("close()");
					exit(EXIT_FAILURE);}
				printf("Ready for new operation...\n");
                                isLocked=false;
				 
		}	
		
		
		
		
		
	}

	exit(EXIT_SUCCESS);
}