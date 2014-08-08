#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *thread_func(void *arg)
{
    printf("msg from main:%s\n",(char *)arg);
    printf("thread id:%u\n",(unsigned)pthread_self());
    pthread_exit("I am thread,exited");
}

int main(int argc,char *argv[])
{
    pthread_t tid;
    void *thread_ret;
    pthread_create(&tid,NULL,thread_func,"I am main");
    pthread_join(tid,&thread_ret);
    printf("msg from thread:%s\n",(char *)thread_ret);
    return 0;
}
