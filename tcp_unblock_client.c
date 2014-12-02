#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXBUF  128
#define handle_error(msg) \
    do{perror(msg);exit(EXIT_FAILURE);}while(0);

int main(int argc,char *argv[])
{
    int sockfd,ret,i;
    struct sockaddr_in dest,mine;
    char buffer[MAXBUF+1];
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
        handle_error("socket");
    memset(&dest,0,sizeof(struct sockaddr_in));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(7575);
    if(inet_aton(argv[1],(struct in_addr *) &dest.sin_addr.s_addr) == 0)
        handle_error("argv[1]");
    if(connect(sockfd,(struct sockaddr *)&dest,sizeof(dest))!=0)
        handle_error("connect");
    if(fcntl(sockfd,F_SETFL,O_NONBLOCK) == -1)
        handle_error("fcntl");
    while(1)
    {
        memset(buffer,'\0',MAXBUF+1);
        ret = recv(sockfd,buffer,MAXBUF,0);
        if(ret>0)
        {
            printf("get %d msg:%s",ret,buffer);
            ret = 0;
        }
        else if(ret < 0)
        {
            if(errno ==EAGAIN)
            {
                errno = 0;
                continue;
            }
            else
                handle_error("recv");
        }
        memset(buffer,'\0',MAXBUF+1);
        printf("input msg to send:");
        fgets(buffer,MAXBUF,stdin);
        if((ret = send(sockfd,buffer,strlen(buffer),0))==-1)
            handle_error("send");
    }
    close(sockfd);
    return 0;
}
