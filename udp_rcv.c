#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define ERROR(msg) \
    do{perror(msg);exit(errno);}while(0)

int main()
{
    struct sockaddr_in s_addr;
    struct sockaddr_in c_addr;
    int sock;
    socklen_t addr_len;
    int len;
    char buff[128];

    if((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1)
        ERROR("socket");
    else
        printf("socket created.\n");

    memset(&s_addr,0,sizeof(struct sockaddr_in));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(7838);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    if((bind(sock,(struct sockaddr *)&s_addr,sizeof(s_addr))) == -1)
        ERROR("bind");
    else
        printf("bind address to socket.\n");
    addr_len = sizeof(c_addr);

    while(1)
    {
        len = recvfrom(sock,buff,sizeof(buff)-1,0,(struct sockaddr *)&c_addr,&addr_len);
        if(len <0)
            ERROR("recvfrom");
        buff[len] = '\0';
        printf("receive from:%s:%d,msg:%s\n\r",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port),buff);
    }
    return 0;
}
