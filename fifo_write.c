#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

#define FIFO "/tmp/fifo"

int main(int argc,char *argv[])
{
    int pipe_fd;
    int res;
    char buffer[]="helloworld!";
    res=mkfifo(FIFO,0766);
    printf("PID %d opening FIFO O_WRONLY.\n",getpid());

    pipe_fd=open(FIFO,O_WRONLY);
    printf("pipe_fd=%d.\n",pipe_fd);
    if(pipe_fd!=-1)
        res=write(pipe_fd,buffer,sizeof(buffer));
    printf("PID %d finished.\n",getpid()); 
    return 0;
}
