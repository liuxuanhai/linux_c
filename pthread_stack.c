#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *tid_func(void *arg)
{
    pthread_attr_t attr;
    size_t stack_size=0;
    pthread_attr_getstacksize(&attr,&stack_size);
    printf("default pthread stack size:%d KB\n",stack_size/1024);
    stack_size=4*1024*1024;
    pthread_attr_setstacksize(&attr,stack_size);
    pthread_attr_getstacksize(&attr,&stack_size);
    printf("after modify,stack size:%d KB\n",stack_size/1024);
    pthread_exit(NULL);
}

int main(int argc ,char *argv[])
{
    pthread_t tid;
    pthread_create(&tid,NULL,tid_func,NULL);
    pthread_join(tid,NULL);
    return 0;
}
