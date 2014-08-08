#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_key_t key;

void *tid1_func(void *arg)
{
    int i=10;
    printf("set key:%d in tid1\n",i);
    pthread_setspecific(key,&i);
    sleep(2);
    printf("in tid1,after tid2 ends,key:%d\n",*(int *)pthread_getspecific(key));
}

void *tid2_func(void *arg)
{
    int j=20;
    printf("set key:%d in tid2\n",j);
    pthread_setspecific(key,&j);
    printf("in tid2,key:%d\n",*(int *)pthread_getspecific(key));
}

void destruct(void *t)
{
    printf("key addr:%p\n",t);
}

int main(int argc,char *argv[])
{
    pthread_t tid1,tid2;
    pthread_key_create(&key,destruct);
    pthread_create(&tid1,NULL,tid1_func,NULL);
    pthread_create(&tid2,NULL,tid2_func,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_key_delete(key);
    return 0;
}
