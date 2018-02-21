#define _POSIX_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int p[2];
int pid,pid1,pid2;
int r;
void sig_handler(int signo)
{
  if (signo == SIGTERM)
    fprintf(stderr,"GEN TERMINATED");
    exit(0);
}

int main()
{

    if(pipe(p)==-1)return 2;

    pid1 = fork();
    if (pid1  == 0) {
        if(dup2(p[1], STDOUT_FILENO)==-1){
            printf("ERROR");
            exit(-1);
        }
        close(p[0]);
        close(p[1]);
        while(1){
            printf("%d %d\n",rand()%10000,rand()%10000);
            fflush(stdout);
            sleep(1);
            if(signal(SIGTERM,sig_handler) == SIG_ERR){
                //printf("dgsls;");
                exit(0);
            }
        }
    }

    pid2=fork();
    if(pid2==0) {

        if(dup2(p[0], STDIN_FILENO)==-1){
            printf("ERROR");
            exit(-1);
        }
        close(p[1]);
        close(p[0]);
        r = execl("nsd", "nsd", NULL);
        if(r ==-1){
            printf("ERROR");
            exit(-1);
        }

    }
    close(p[0]);
	close(p[1]);
	if(pid1==-1)return 2;
	if(pid2==-1)return 2;
    sleep(5);
	if(pid1!=-1){
		if(kill(pid1,SIGTERM)!=0){
		    /*fprintf(stderr,"ERROR");
		    return 0;*/
		}
	}

	int status;
	waitpid(pid1,&status,0);
	if(status==65280)return 1;
	waitpid(pid2,&status,0);
	if(status==65280)return 1;
    printf("OK\n");
    return 0;

}
