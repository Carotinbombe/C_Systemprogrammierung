#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>


#define BUFFERSIZE 1024
#define MSG_SERV 100
#define MSG_CLNT 99
#define DATA_PATH 1
#define DATA_SUCC 2
#define DATA_FAIL -1
#define DATA_SEND 3




int msqid;
int fd_src;

/*Exithandler schliesst Msg Queue und Filedeskriptor*/
void exitHandler(){
    if(msgctl(msqid, IPC_RMID,NULL) < 0){
        perror("mesgctl()");
        _exit(EXIT_FAILURE);
    }
	if(close(fd_src) < 0){
		perror("close()");
        _exit(EXIT_FAILURE);}
    printf("\nclosing queue successfully\n");
}
/*Interrupt durch ^C ruft Exithandler auf*/
void Interrupt(){
    exitHandler();
    _exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]){
 
 /*Struct für Message Queue*/
	struct msgbuffer{
        long mtype;
        char mtext[BUFFERSIZE];
        ssize_t size;
        long datatype;
    }msg_client,msg_server;
    
    
    /*Server Key*/
    char *serverkey = "./server";
    key_t key;
    /*Interrupt*/
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler=Interrupt;
  
    
    /*Signal erzeugen ^C*/
    if(sigaction(SIGINT,&act,NULL) < 0){
            perror("sigaction()");
            return EXIT_FAILURE;}
            
   /*Key erzeugen*/
    if((key = ftok(serverkey,1)) < 0){
        perror("ftok()");
        return EXIT_FAILURE;}
    
    /*Messagequeue erstellen*/
    if((msqid = msgget(key,IPC_CREAT | 0600)) < 0){
         perror("msgget()");
         return EXIT_FAILURE;}
         
    printf("message queue was created with msqid: %i\t key: 0x%08x\n",msqid,key);
    /*Aktiviere exitHandler*/
    atexit(exitHandler);
   
   while(1){
	 /*Warte auf Message vom Client, wenn Fehler -> EXIT */
		if(msgrcv(msqid,&msg_client,sizeof(msg_client),MSG_CLNT,0) < 0){
		perror("msgrcv()");
                return EXIT_FAILURE;}
		/*Unterscheidung nach Empfangen Typen*/
		
                switch(msg_client.datatype){
			/*Pfad empfangen*/
			case DATA_PATH:	
				printf("copy at %s\n",msg_client.mtext);
				/*Erstelle Datei-> Fehler wenn Datei vorhanden*/
				if((fd_src=open(msg_client.mtext, O_RDWR | O_CREAT | O_EXCL)) < 0){
					perror("fehler");
					
					/*Struct vorbereiten*/
					msg_server.datatype=DATA_FAIL;
					msg_server.mtype=MSG_SERV;
					
					/*Wenn Datei existiert*/
					if(errno == EEXIST){strcpy(msg_server.mtext,"file already exist\n");}
					
					/*Wenn Datei nicht anleget oder andere Fehler auftreten */
					else {strcpy(msg_server.mtext,"error with open stream!\n");}
					msg_server.size=sizeof(msg_server.mtext);
					/*Gebe Client Bewscheid*/
					if(msgsnd(msqid,&msg_server,sizeof(msg_server),0) < 0){
						perror("Message send failure");}
						exit(EXIT_FAILURE);}
				/*Datei angelegt und Client ACK gesendent*/
				else{
					
					msg_server.datatype=DATA_SUCC;
					msg_server.mtype=MSG_SERV;
					if(msgsnd(msqid,&msg_server,sizeof(msg_server),0) < 0){
						perror("Message send failure");
                                                exit(EXIT_FAILURE);}
				}
				
				break;
			/*Empfange Daten*/
			case DATA_SEND:
				/*Schreibe in Datei*/
				if (write(fd_src,msg_client.mtext,msg_client.size) < 0){
					perror("write()");
                                        exit(EXIT_FAILURE);}
				
				printf("Data write successfully\n");
				/*ACK zum Client*/
				msg_server.datatype=DATA_SUCC;
				msg_server.mtype=MSG_SERV;
					if(msgsnd(msqid,&msg_server,sizeof(msg_server),0) < 0){
                                        perror("Message send failure");
                                        exit(EXIT_FAILURE);}
				break;
			
			/*Schliesse Filedeskriptor*/
			case DATA_SUCC:
				if(close(fd_src) < 0){
					perror("close()");
					exit(EXIT_FAILURE);}
				printf("Ready for new Operation...\n");
				 
		}	
			
		
		
         
    }
  
    
    exit(EXIT_SUCCESS);
}