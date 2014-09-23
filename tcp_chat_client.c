#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXBUF  1024

int main(int argc,char *argv[])
{
    int sockfd,len;
    struct sockaddr_in  dest;
    char buffer[MAXBUF+1];
    if(argc!=3)
    {
        printf("usage:./%s IP port",argv[0]);
        exit(-1);
    }
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket");
        exit(-1);
    }
    printf("socket created\n");
    memset(&dest,0,MAXBUF+1);
    dest.sin_family=AF_INET;
    dest.sin_port=htons(atoi(argv[2]));
    if(inet_aton(argv[1],(struct in_addr *)&dest.sin_addr.s_addr)==0)
    {
        perror("argv[1]");
        exit(-1);
    }
    if(connect(sockfd,(struct sockaddr *)&dest,sizeof(dest))==-1)
    {
        perror("connect");
        exit(-1);
    }
    printf("Server connected.\n");
    pid_t pid;
    if((pid=fork())==-1)
    {
        perror("fork");
        exit(-1);
    }
    else if(pid==0)//child.Recv mesg.
    {
        while(1)
        {
            memset(buffer,0,MAXBUF+1);
            len=recv(sockfd,buffer,MAXBUF,0);
            if(len>0)
                printf("client recv:%s",buffer);
            else if(len<0)
            {
                perror("recv");
                break;
            }
            else
            {
                printf("server closed.quit\n");
                break;
            }
        }
    }
    else //parent.send mesg.
    {
        while(1)
        {
            memset(buffer,0,MAXBUF+1);
            printf("client mesg to send:");
            fgets(buffer,MAXBUF,stdin);
            if(!strncasecmp(buffer,"quit",4))
            {
                printf("quiting the chat.\n");
                break;
            }
            len=send(sockfd,buffer,strlen(buffer)-1,0);
            if(len<0)
            {
                perror("send");
                break;
            }
        }
        wait(NULL);
    }
    close(sockfd);
    return 0;
}

