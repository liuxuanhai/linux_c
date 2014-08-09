#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void sig_handler(int signo)
{
    printf("tid=%u,sig=%d\n",(unsigned int)pthread_self(),signo);
}

void *tid1_func(void *arg)
{
    int i;
    sigset_t set;
    signal(SIGUSR1,sig_handler);
    sigfillset(&set);
    sigdelset(&set,SIGUSR2);
    pthread_sigmask(SIG_SETMASK,&set,NULL);
    for(i=0;i<5;i++)
    {
        printf("tid1=%u, set mask.\n",(unsigned int)pthread_self());
        pause();
    }
}

void *tid2_func(void *arg)
{
    int i;
    signal(SIGUSR2,sig_handler);
    for(i=0;i<5;i++)
    {
        printf("tid2=%u, not set mask.\n",(unsigned int)pthread_self());
        pause();
    }
}

int main(int argc ,char *argv[])
{
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,tid1_func,NULL);
    pthread_create(&tid2,NULL,tid2_func,NULL);
    sleep(1);
    pthread_kill(tid1,SIGUSR1);
    pthread_kill(tid1,SIGUSR2);
    pthread_kill(tid2,SIGUSR1);
    pthread_kill(tid2,SIGUSR2);
    sleep(1);
    pthread_kill(tid1,SIGKILL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}
