#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <resolv.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define HANDLE_ERROR(msg) \
    do{perror(msg);exit(EXIT_FAILURE);}while(0)
#define MAXBUF  1024
#define SERVER_PORT 7575
#define SERVER_IP "127.0.0.1"

int main(int argc,char *argv[])
{
    int sockfd,len,ret,maxfd=-1;
    struct sockaddr_in  dest;
    char buffer[MAXBUF+1];
    fd_set  rfds;
    struct timeval tv;
    
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    printf("socket created\n");
    memset(&dest,'\0',sizeof(struct sockaddr_in));
    dest.sin_family=AF_INET;
    dest.sin_port=htons(SERVER_PORT);
    inet_aton(SERVER_IP,(struct in_addr *)&dest.sin_addr.s_addr);
    ret = connect(sockfd,(struct sockaddr *)&dest,sizeof(dest));
    if(ret)
       HANDLE_ERROR("connect"); 
    printf("Server connected.\n");
    while(1)
    {
        FD_ZERO(&rfds);
        FD_SET(0,&rfds);
        FD_SET(sockfd,&rfds);
        maxfd = sockfd;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        ret = select(maxfd+1,&rfds,NULL,NULL,&tv);
        if(ret == -1)
            HANDLE_ERROR("select");
        else if(ret == 0)
            continue;
        else
        {
            if(FD_ISSET(sockfd,&rfds))
            {
                memset(buffer,0,MAXBUF+1);
                len = recv(sockfd,buffer,MAXBUF,0);
                if(len>0)
                    printf("recv:%s\n",buffer);
                else if(len<0)
                    printf("recv FAILED.\n");
                else
                {
                    printf("Server ends chat normally.\n");
                    break;
                }
            }
            if(FD_ISSET(0,&rfds))
            {
                memset(buffer,0,MAXBUF+1);
                fgets(buffer,MAXBUF,stdin);
                len = send(sockfd,buffer,strlen(buffer)-1,0);
                if(len<0)
                    HANDLE_ERROR("send");
            }
        }
    }
    close(sockfd);
    return 0;
}


