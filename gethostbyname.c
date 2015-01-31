#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

extern int h_errno;
#define ERROR(msg) \
    do{perror(msg);exit(errno);}while(0)

int main(int argc,char *argv[])
{
    int idx = 0;
    struct hostent *hp;
    extern h_errno;
    if(argc!=2)
    {
        printf("usage:./gethostbyname [name]\n");
        exit(0);
    }
    hp = gethostbyname(argv[1]);
    if(hp == NULL)
    {
        printf("%s\n",hstrerror(h_errno)); 
        exit(0);
    }
    printf("Host name:\t%s\n",argv[1]);
    printf("Official Name:\t%s\n",hp->h_name);
    for(idx = 0;hp->h_aliases[idx] != NULL;idx++)
        printf("Alias %d:\t%s\n",idx,hp->h_aliases[idx]);
    if(hp->h_addrtype == AF_INET)
        printf("INET Type:\tAF_INET\n");
    else
        printf("INET Type:\tAF_INET6\n");
    for(idx=0;hp->h_addr_list[idx];idx++)
        printf("Address %d:\t%s\n",idx,inet_ntoa(*(struct in_addr *)hp->h_addr_list[idx]));
    return 0;
}

