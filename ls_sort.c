#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc,char *argv[])
{
    int fd[2];
    if(pipe(fd)==-1)
    {
        perror("pipe");
        exit(-1);
    }
    if(fork() == 0)//child process.用于sort
    {
        char buf[128];
        dup2(fd[0],0);
        close(fd[1]);
        execlp("sort","sort",(char *)0);
    }
    else
    {
        if(fork()==0)//child process.用于ls
        {
            dup2(fd[1],1);
            close(fd[0]);
            execlp("ls","ls","/home",(char *)0);
        }
        else
        {
            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}
