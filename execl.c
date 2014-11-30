#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv)
{
    int status;
    execl("/bin/ls","ls","-al",NULL);
    return 0;
}
