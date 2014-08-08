//msg_receiver
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#define KEY_PATH  "/home/tao"
struct msgbuf{
    int type;
    char ptr[0];
};

int main(int argc,char *argv[])
{
    key_t key;
    key=ftok(KEY_PATH,100);
    int msgid;
    msgid=msgget(key,IPC_CREAT|0600);
    printf("msgid=%d\n",msgid);
    pid_t pid;
    pid=fork();
    if(pid==0)//子进程发送消息
    {
        while(1)
        {
            printf("input msg to be sent:");
            char buf[128];
            fgets(buf,128,stdin);
            struct msgbuf *ptr = malloc(sizeof(struct msgbuf)+strlen(buf)+1);
            ptr->type=2; //发送消息类型为2的消息
            memcpy(ptr->ptr,buf,strlen(buf)+1);
            msgsnd(msgid,ptr,strlen(buf)+1,0);
            free(ptr);
        }
    }
    else
    {
        struct msgbuf{
            int type;
            char ptr[1024];
        };
        while(1)
        {
            struct msgbuf mybuf;
            memset(&mybuf,'\0',1024);
            int ret=msgrcv(msgid,&mybuf,1024,1,0);//接受消息类型为1的消息
            printf("receive msg:%s\n",mybuf.ptr);
        }
    }
    return 0;
}
