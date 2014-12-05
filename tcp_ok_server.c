#include <sys/socket.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#define HANDLE_ERROR(msg) \
    do{perror(msg);exit(EXIT_FAILURE);}while(0)
#define MAXBUF  1024
#define PORT    7575
#define SERVER_IP   "127.0.0.1"

int main(int argc,char *argv[])
{
    int sockfd,new_fd,ret,maxfd = -1;
    socklen_t   len;
    struct sockaddr_in my_addr,their_addr;
    unsigned int lisnum=5;
    unsigned int myport=PORT;
    char buf[MAXBUF+1];
    fd_set  rfds;
    struct timeval tv; 

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(myport);
    my_addr.sin_addr.s_addr=inet_addr(SERVER_IP);
    bind(sockfd,(struct sockaddr *) &my_addr,sizeof(struct sockaddr));
    listen(sockfd,lisnum);

    while(1)
    {
        printf("waiting for client...\n");
        len=sizeof(struct sockaddr);
        new_fd=accept(sockfd,(struct sockaddr *)&their_addr,&len);
        printf("Client %s:%d connected on socket %d\n",(char *)inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port),new_fd);
        while(1)
        {
            FD_ZERO(&rfds);
            FD_SET(0,&rfds);
            FD_SET(new_fd,&rfds);
            maxfd = new_fd;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            ret = select(maxfd+1,&rfds,NULL,NULL,&tv);
            if(ret == -1)
                HANDLE_ERROR("select");
            else if(ret ==0)
                continue;
            else
            {
                if(FD_ISSET(0,&rfds))
                {
                    memset(buf,'\0',MAXBUF+1);
                    fgets(buf,MAXBUF,stdin);
                    len = send(new_fd,buf,strlen(buf)-1,0);
                    if(len<=0)
                    {
                        printf("send FAILED.\n");
                        break;
                    }
                }
                if(FD_ISSET(new_fd,&rfds))
                {
                    memset(buf,'\0',MAXBUF+1);
                    len = recv(new_fd,buf,MAXBUF,0);
                    if(len>0)
                        printf("Recv:%s\n",buf);
                    else if(len<0)
                        printf("recv FAILED.\n");
                    else
                    {
                        printf("Client ends chat normally.\n");
                        break;
                    }
                }
            }
        }
        close(new_fd);
        fflush(stdout);//clear screen and wait for next client.
        memset(buf,'\0',MAXBUF+1);
    }
    close(sockfd);
    return 0;
}

