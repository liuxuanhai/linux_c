#include <sys/socket.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAXBUF 1024

int main(int argc,char *argv[])
{
    int pid;
    int sockfd,new_fd;
    socklen_t   len;
    struct sockaddr_in my_addr,their_addr;
    unsigned int myport,lisnum;
    char buf[MAXBUF+1];
    if(argv[2])
        myport=atoi(argv[2]);
    else
        myport=7575;
    if(argv[3])
        lisnum=atoi(argv[3]);
    else
        lisnum=5;
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket");
        exit(-1);
    }
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(myport);
    if(argv[1])
        my_addr.sin_addr.s_addr=inet_addr(argv[1]);
    else
        my_addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(sockfd,(struct sockaddr *) &my_addr,sizeof(struct sockaddr))==-1)
    {
        perror("bind");
        exit(-1);
    }
    if(listen(sockfd,lisnum)==-1)
    {
        perror("listen");
        exit(-1);
    }
    printf("waiting for connection...\n");
    len=sizeof(struct sockaddr);
    if((new_fd=accept(sockfd,(struct sockaddr *)&their_addr,&len))==-1)
    {
        perror("accept");
        exit(-1);
    }
    else
        printf("server:got connect from %s,port:%d,socket:%d\n",inet_ntoa(their_addr.sin_addr),ntohs(their_addr.sin_port),new_fd);
    if((pid=fork())==-1)
    {
        perror("fork");
        exit(-1);
    }
    else if(pid==0)//child:send msg.
    {
        while(1)
        {
            memset(buf,0,MAXBUF+1);    
            printf("msg to send:");
            fgets(buf,MAXBUF,stdin);
            len=send(new_fd,buf,strlen(buf)-1,0);
            if(len<0)
            {
                printf("msg send failed.\n");
                break;
            }
        }
    }
    else        //parent:receive msg.
    {
       while(1)
       {
           memset(buf,0,MAXBUF+1);
           len=recv(new_fd,buf,MAXBUF,0);
           if(len>0)
               printf("msg recved:%s\n",buf);
           else if(len<0)
           {
               printf("recv failed.\n");
               break;
           }
       }
    }
    close(new_fd);
    close(sockfd);
    return 0;
}
