#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int signo)
{
    printf("signo=%d\n",signo);
}
int main()
{
    if(signal(SIGUSR1,sig_handler)==SIG_ERR)
    {
        printf("can't catch SIGUSR1\n");
    }
    pause();
    return 0;
}
