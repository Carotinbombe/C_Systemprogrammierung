#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <signal.h>
#include "settings.h"


int fd_src;
int socketfd;

/*Exithandler schliesst Msg Queue und Filedeskriptor*/
void exitHandler(){
   printf("-----EXIT-HANDLER-----\n");
   if(close(fd_src) < 0){
            perror("close()");
            _exit(EXIT_FAILURE);}
    printf("\nclosed Filedescriptor\n");
	
	if(close(socketfd) < 0){
		perror("close()");
        _exit(EXIT_FAILURE);}
    printf("closed socket fd\n");
	
	if(unlink(SOCKET_PATH) < 0 ){
        perror("unlink()");
        _exit(EXIT_FAILURE);}
    printf("unlinked socket\n");
	
}
/*Interrupt durch ^C ruft Exithandler auf*/
void Interrupt(){
    exitHandler();
    _exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]){
 

    struct sockaddr_un server_adress,client_address;
    struct SocketMessage msg_server;
    char *server = SOCKET_PATH;
    
	socklen_t clientaddrlen;
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler=Interrupt;
	/* Länge der Struktur für Peer */
    clientaddrlen = sizeof(struct sockaddr_un);
    
    /*Signal erzeugen ^C*/
    if(sigaction(SIGINT,&act,NULL) < 0){
            perror("sigaction()");
            return EXIT_FAILURE;}
            
    printf("Siganction activated\n");  
    /*Aktiviere exitHandler*/
    atexit(exitHandler);
   
    printf("Exit handler activated\n");
   /*Socket erstellen*/ 
    if((socketfd = socket(PF_LOCAL, SOCK_STREAM,0)) < 0){
        perror("socket()");
        return EXIT_FAILURE;}
   
    printf("Socket activated\n");
   /*Struct vorbereiten*/    
    server_adress.sun_family = PF_LOCAL;
    strncpy(server_addr.sun_path,  SOCKET_PATH, sizeof(server_addr.sun_path)-1);
    addrlen = strlen(server_addr.sun_path) + offsetof(struct sockaddr_un, sun_path);
    
    
        
    /*Socket binden*/
    if(bind(socketfd,(struct sockaddr*) &server_adress, sizeof(server_adress)) < 0){
        perror("bind()");
        return EXIT_FAILURE;}
    
    printf("Socket binded\n");
    if((listen(socketfd,QUEUE_VALUE)) < 0){
        perror("listen()");
        return EXIT_FAILURE;}
    
	printf("Listen set to %u\n\n",QUEUE_VALUE);
   
   while(1){
	    printf("----Welcome---\n\n");
	    
		printf("Wating for client connection\n");
		/* auf Auftrag warten */
        if(accept(socketfd, (struct sockaddr *)&client_address, &clientaddrlen) < 0) {
              perror("accept()");
              exit(EXIT_FAILURE);}
	 
	 /*Warte auf Message vom Client, wenn Fehler -> EXIT */
		if((recv(socketfd, &msg_server, sizeof(struct SocketMessage), 0)) < 0){
				perror("Message receive error");
				exit(EXIT_FAILURE);}
				
		/*Unterscheidung nach Empfangen Typen*/
		
        switch(msg_server.msg_typ){
			/*Pfad empfangen*/
			case DATA_PATH:	
				printf("copy at %s\n",msg_server.msg_text);
				/*Erstelle Datei-> Fehler wenn Datei vorhanden*/
				if((fd_src=open(msg_server.msg_text, O_RDWR | O_CREAT | O_EXCL)) < 0){
					perror("fehler");
					
					/*Struct vorbereiten*/
					msg_server.msg_typ=DATA_FAIL;
					
					/*Wenn Datei existiert*/
					if(errno == EEXIST){strcpy(msg_server.msg_text,"file already exist\n");}
					
					/*Wenn Datei nicht anleget oder andere Fehler auftreten */
					else {strcpy(msg_server.msg_text,"error with open stream!\n");}
					msg_server.msg_size=sizeof(msg_server.msg_text);
					
					/*Gebe Client Bescheid*/
					if(send(socketfd, &msg_server,sizeof(msg_server),0) < 0){
						perror("Message send failure");}
						exit(EXIT_FAILURE);}
				/*Datei angelegt und Client ACK gesendent*/
				else{
					
					msg_server.msg_typ=DATA_SUCC;
					if(send(socketfd, &msg_server,sizeof(msg_server),0) < 0){
						perror("Message send failure");}
						exit(EXIT_FAILURE);}
				
				
				break;
			/*Empfange Daten*/
			case DATA_SEND:
				/*Schreibe in Datei*/
				if (write(fd_src,msg_server.msg_text,msg_server.msg_size) < 0){
					perror("write()");
                    exit(EXIT_FAILURE);}
				
				printf("Data write successfully\n");
				/*ACK zum Client*/
				msg_server.msg_typ=DATA_SUCC;
				if(send(socketfd, &msg_server,sizeof(msg_server),0) < 0){
						perror("Message send failure");
						exit(EXIT_FAILURE);}
				break;
			
			/*Schliesse Filedeskriptor*/
			case DATA_SUCC:
				if(close(fd_src) < 0){
					perror("close()");
					exit(EXIT_FAILURE);}
				printf("Ready for new operation...\n");
				 
		}	
			
		
		
         
    }
  
    
    exit(EXIT_SUCCESS);
}