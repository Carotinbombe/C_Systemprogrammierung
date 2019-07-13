#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

/*Statische Funktion für Signalhandling*/
static void sig_user1(int sig);

int main (int argc, char *argv[]){
        pid_t pid;
	/*Erstelle Signal und lege Eigenschaften fest*/
	struct sigaction act;
	act.sa_handler= sig_user1;
	act.sa_flags = 0;
	
	
	//Signalmenge erzeugen 
	if (sigemptyset(&act.sa_mask) < 0) { 
		perror("SigEmptySet : ");
		exit(EXIT_FAILURE);}
	/*Signal erzeugen*/
	if(sigaction(SIGUSR1,&act,NULL) < 0){
		perror("sigaction()");
		return EXIT_FAILURE;}
	
	/*------*/
	/* Fork */
	/*------*/
    switch(pid=fork()){
        /*Kindprozess*/
		case 0:	
			for(int i = 0; i < 20 ; i++){
				/*Warte auf Elternprozess*/
				pause();
				
				printf("Kindprozess PID: \t%d\n",getpid());
				/*Sende Signal zu Elternprozess*/
				if (kill(getppid(),SIGUSR1) < 0) {
					perror("Child kill");
					exit(EXIT_FAILURE);}
				}
			break;
        /*Fehler*/
        case -1:
            perror("Fehler bei fork()");
            return EXIT_FAILURE;
        /*Elternprozess*/
        default:
                    sleep(1);
            for(int i = 0; i < 20 ; i++){
				printf("Elternprozess PID: \t%d\n",getpid());
				/*Sende Signal an Kind*/
				if (kill(pid,SIGUSR1) < 0) {
					perror("parent kill ");
					exit(EXIT_FAILURE);}
				/*Warte bis vom Kind Signal kommt*/	
				pause();}
			
        	/*Warte am Ende auf Kindprozess*/
			if (waitpid(pid, NULL, 0) < 0) {
			   perror("waitpid");
			   exit(EXIT_FAILURE);}    
    }

    return EXIT_SUCCESS;
}
/*Signalroutine*/
static void sig_user1(int sig){}