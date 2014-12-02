#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <netinet/in.h>
#include <errno.h>

#define BUFSIZE 128
#define handle_error(msg)   \
    do{perror(msg);exit(EXIT_FAILURE);}while(0);
int main(int argc,char *argv[])
{
    int server_sockfd,client_sockfd;
    socklen_t server_len,client_len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int i,byte;
    char buffer_send[BUFSIZE];
    server_sockfd=socket(AF_INET,SOCK_STREAM,0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7575);
    if(inet_aton(argv[1],(struct in_addr *)&server_addr.sin_addr.s_addr)==0)
        handle_error(argv[1]);
    server_len=sizeof(struct sockaddr);
    if(bind(server_sockfd,(struct sockaddr *) &server_addr,server_len)!=0)
        handle_error("server bind");
    if(listen(server_sockfd,5)!=0)
        handle_error("server listen");
    printf("server waiting for connect...\n");
    client_len=sizeof(struct sockaddr);
    client_sockfd=accept(server_sockfd,(struct sockaddr *)&client_addr,(socklen_t *) &client_len);
    printf("client connected\n");
    
    for(i=0;i<5;i++)
    {
        memset(buffer_send,'\0',BUFSIZE);
        printf("to send:");
        fgets(buffer_send,BUFSIZE,stdin);
        if(byte=send(client_sockfd,buffer_send,strlen(buffer_send),0)==-1)
            handle_error("send");
        memset(buffer_send,'\0',BUFSIZE);
        byte = recv(client_sockfd,buffer_send,BUFSIZE,MSG_DONTWAIT);
        if(byte>0)
        {
            printf("get %d msg:%s",byte,buffer_send);
            byte=0;
        }
        else if(byte==0)
        {
            if(errno== EAGAIN )
            {
                errno=0;
                continue;
            }
            else
                handle_error("recv");
        }
    }
    shutdown(client_sockfd,2);
    shutdown(server_sockfd,2);
}
