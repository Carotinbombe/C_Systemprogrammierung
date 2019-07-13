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
#include <pthread.h>
#include "settings.h"


int fd_src;
int fd_sock;

struct pthread_arg{
    int fd_src;
    int commsockfd;
};
    

void* connectionHandler(void* socket);

/*Exithandler schliesst Socket und Filedeskriptor*/
void exitHandler(){
   printf("-----EXIT-HANDLER-----\n\n");
   if(unlink(SOCKET_PATH) < 0 ){
        perror("unlink()");
        _exit(EXIT_FAILURE);}
    printf("unlinked socket\n");
	
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
     
    /*Thread*/
    pthread_t thread;

    
	
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
    if(listen(fd_sock, 50) < 0) {
        perror("listen()");
        exit(EXIT_FAILURE);}
    
    while(1){
     	/* auf Auftrag warten */
        if((commsockfd = accept(fd_sock, NULL, NULL)) < 0){
            perror("accept()");
            exit(EXIT_FAILURE);}
        /*Malloc für Pointer auf ein Socketdes*/
        int* clientsocketdes = malloc(sizeof(int));
        if(clientsocketdes == NULL){
            perror("malloc())");
            exit(EXIT_FAILURE);
        }
        /*Kopiere*/
        if(memcpy(clientsocketdes, &commsockfd, sizeof(int)) == NULL){
            perror("memset()");
            exit(EXIT_FAILURE);}
        
        printf("%p\n",clientsocketdes);
        
        /* Thread erzeugen */  
        if(pthread_create(&thread,NULL,&connectionHandler,(void *) clientsocketdes) < 0){
            perror("thread()");
            exit(EXIT_FAILURE);}
        if(pthread_detach(thread) < 0){
            perror("detach()");
            exit(EXIT_FAILURE);}
        
        }
    
    
    
    exit(EXIT_SUCCESS);
}

void thread_exit(void *arg){
	printf("-----EXIT-HANDLER-----\n");
    struct pthread_arg args = *(struct pthread_arg *)arg;
    if(close(args.commsockfd) < 0){
        perror("Thread: close");}
    printf("Thread: Communication socket was closed\n");
    if(close(args.fd_src) <0 ){
        perror("Thread: close");}
    printf("Thread: Filedescriptor  was closed\n");
    
}



void* connectionHandler(void* socket)
{
    printf("Yuhu ich bin ein Thread!\n");
   
    /*Message*/
    struct SocketMessage server;
    /*Struct für Thread*/
    struct pthread_arg arg;
    /*Parameter übergeben*/
    arg.commsockfd = *((int*) socket);
    free((int*)socket);
 
    /*Exit-Handler für Thread*/
    pthread_cleanup_push(thread_exit,(void *)&arg);
    
    int run = 1;
    while(run){
            if(recv(arg.commsockfd, &server, sizeof(server), 0) < 0){
                perror("Thread: recv()");
                pthread_exit(NULL);}
            
		switch(server.msg_typ){
			/*Pfad empfangen*/
			case DATA_PATH:	
				printf("Thread: copy to path: %s\n",server.msg_text);
				/*Erstelle Datei-> Fehler wenn Datei vorhanden*/
				if((arg.fd_src=open(server.msg_text, O_RDWR | O_CREAT | O_EXCL)) < 0){
					perror("fehler");
					
					/*Struct vorbereiten*/
					server.msg_typ=DATA_FAIL;
					
					/*Wenn Datei existiert*/
					if(errno == EEXIST){strcpy(server.msg_text,"file already exist\n");}
					
					/*Wenn Datei nicht anleget oder andere Fehler auftreten */
					else {strcpy(server.msg_text,"Thread: error with open stream!\n");}
					server.msg_size=sizeof(server.msg_text);
					
					/*Gebe Client Bescheid*/
					if(send(arg.commsockfd, &server,sizeof(server),0) < 0){
						perror("Thread: Message send failure");}
						 pthread_exit(NULL);}
				
				/*Datei angelegt und Client ACK gesendent*/
				else{
					printf("Thread: File created\nThread: Sending ACK\n");
					server.msg_typ=DATA_SUCC;
                                        if(send(arg.commsockfd, &server,sizeof(server),0) < 0){
						perror("Message send failure");
						 pthread_exit(NULL);}
                                        printf("Thread: sended ACK\n");}
				
					
				break;
			/*Empfange Daten*/
			case DATA_SEND:
				/*Schreibe in Datei*/
				if (write(arg.fd_src,server.msg_text,server.msg_size) < 0){
					perror("Thread: write()");
                                        pthread_exit(NULL);}
				
				printf("Thread: Data write successfully\n");
				
				/*ACK zum Client*/
				server.msg_typ=DATA_SUCC;
				if(send(arg.commsockfd, &server,sizeof(server),0) < 0){
						perror("Thread: Message send failure");
                        pthread_exit(NULL);}
				printf("Send ACK\n");
							
				break;
			
			/*Schliesse Filedeskriptor*/
			case DATA_SUCC:
                                printf("Thread: Success!\n");
                                run = 0;
                                
				 
		}	
    }
    /*Führe Exit-Handler auf*/
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
    
    
}