#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define FIFO "/tmp/fifo"

int main(int argc,char *argv[])
{
    int pipe_fd;
    int bytes_read=0;
    char buffer[4096];
    memset(buffer,'\0',4096);
    printf("PID %d opening FIFO O_WRONLY.\n",getpid());

    pipe_fd=open(FIFO,O_RDONLY);
    printf("pipe_fd=%d.\n",pipe_fd);
    if(pipe_fd!=-1)
    {
        bytes_read=read(pipe_fd,buffer,sizeof(buffer));
        printf("read data:%s\n",buffer);
    }
    printf("PID %d finished.\n",getpid()); 
    return 0;
}
