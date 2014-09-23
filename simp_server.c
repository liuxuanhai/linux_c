#include <sys/socket.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAXBUF  1024
#define PORT    7575
#define SERVER_IP   "127.0.0.1"
int main(int argc,char *argv[])
{
    int sockfd,new_fd;
    socklen_t   len;
    struct sockaddr_in my_addr,their_addr;
    unsigned int lisnum=5;
    unsigned int myport=PORT;
    char buf[MAXBUF+1];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(myport);
    my_addr.sin_addr.s_addr=inet_addr(SERVER_IP);
    bind(sockfd,(struct sockaddr *) &my_addr,sizeof(struct sockaddr));
    listen(sockfd,lisnum);
    printf("waiting for client...\n");
    len=sizeof(struct sockaddr);
    new_fd=accept(sockfd,(struct sockaddr *)&their_addr,&len);
    while(1)
    {
        memset(buf,0,MAXBUF+1);
        len=recv(new_fd,buf,MAXBUF,0);
        if(len>0)
            printf("recved msg:%s\n",buf);
        else if(len<0)
        {
            printf("recv failed.\n");
            break;
        }
    }
    close(new_fd);
    close(sockfd);
    return 0;
}
