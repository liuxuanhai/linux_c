#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int signo,siginfo_t *info,void *p)
{
    printf("signo=%d\n",signo);
    printf("siginfo->signo:%d\n",info->si_signo);
    printf("sender pid=%d\n",info->si_pid);
}
int main()
{
    struct sigaction act,oldact;
    sigemptyset(&act.sa_mask);
    act.sa_flags=SA_SIGINFO;
    act.sa_sigaction=sig_handler;
    sigaction(SIGUSR1,&act,&oldact);
    pause();
    return 0;
}
