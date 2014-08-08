#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sighandler(int signo)
{
    printf("caught signo:%d\n",signo);
}
int main()
{
    sigset_t newmask,oldmask,pendmask;
    //安装SIGUSR1的处理函数
    signal(SIGUSR1,sighandler);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGUSR1);
    //屏蔽SIGUSR1
    sigprocmask(SIG_BLOCK,&newmask,&oldmask);
    printf("SIGUSR1 blocked for 20s.\n");
    sleep(20);
    //进程可以收到SIGUSR1，但是被屏蔽了，处于pending状态，
    //sigpending可以查看处于pending的所有信号。
    sigpending(&pendmask);
    if(sigismember(&pendmask,SIGUSR1))
        printf("SIGUSR1 is pending...\n");
    //取消屏蔽SIGUSR1
    printf("SIGUSR1 unblocked.\n");
    sigprocmask(SIG_SETMASK,&oldmask,NULL);
    return 0;
}

