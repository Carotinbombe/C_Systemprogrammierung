#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>



int main (int argc, char *argv[]){
    pid_t pid;
    pid_t child;
    int status;
    
    switch(pid=fork()){
        case 0:
            printf("Ich bin ein Kindprozess!\n");
            printf("PID: %d \t PPID: %d\n",getpid(),getppid());
            for(int i=0;i<1000000000;i++){(i*i*i*0.55*6)/((i/i*8)*i);}
            break;
        
        case -1:
            perror("Fehler bei fork()");
            return EXIT_FAILURE;
        
        default:
            printf("Ich bin ein Elternprozess!\n");
            printf("PID: %d \t PPID: %d\n",getpid(),getppid());
            for(int i=0;i<1000000000;i++){(i*i*i*0.55*6)/((i/i*8)*i);}
            printf("Eltern ist fertig!\n");
            
            if((child=waitpid(pid, &status, WNOHANG)) < 0 ){
                perror("waitpid()");
                return EXIT_FAILURE;}
            
            else if(child == 0)
                printf("Kindprozess  wurde beendet!\n");
            else
                printf("Kindprozess läuft...\n");
            
            
    }
    
    
 
    return EXIT_SUCCESS;
}