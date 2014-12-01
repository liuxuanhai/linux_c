#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXBUF  1024
#define PORT    7575
//#define SERVER_IP "127.0.0.1"
#define SERVER_IP "192.168.2.118"
int main(int argc,char *argv[])
{
    int sockfd,len,ret;
    struct sockaddr_in  dest;
    char buffer[MAXBUF+1];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    printf("socket created\n");
    memset(&dest,0,sizeof(struct sockaddr_in));
    dest.sin_family=AF_INET;
    dest.sin_port=htons(PORT);
    inet_aton(SERVER_IP,(struct in_addr *)&dest.sin_addr.s_addr);
    ret = connect(sockfd,(struct sockaddr *)&dest,sizeof(dest));
    if(ret)
        printf("connect failed.\n");
    printf("Server connected.\n");
    while(1)
    {
        memset(buffer,0,MAXBUF+1);
        printf("client msg to send:");
        fgets(buffer,MAXBUF,stdin);
        if(!strncasecmp(buffer,"quit",4))
        {
            printf("quiting the chat.\n");
            break;
        }
        len=send(sockfd,buffer,strlen(buffer)-1,0);
    }
    close(sockfd);
    return 0;
}

