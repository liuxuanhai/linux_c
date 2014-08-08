#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>

int main(int argc,char *argv[])
{
    int running = 1;
    int shmid;
    int semid;
    int value;
    void *share_memp=NULL;
    struct sembuf sem;
    sem.sem_num=0;
    sem.sem_flg=SEM_UNDO;
    semid=semget((key_t)123456,1,0666|IPC_CREAT);
    //设置信号初始值为0
    semctl(semid,0,SETVAL,0);

    shmid=shmget((key_t)654321,(size_t)2048,0600|IPC_CREAT);
    share_memp = shmat(shmid,NULL,0);
    while(running)
    {
        value=semctl(semid,0,GETVAL);
        if(value==1)
        {
            printf("read data operate\n");
            sem.sem_op=-1;
            semop(semid,&sem,1);
            printf("%s",(char *)share_memp);
        }
        if(strcmp(share_memp,"end")==0)
            running--;
    }
    shmdt(share_memp);
    shmctl(shmid,IPC_RMID,0);
    semctl(semid,IPC_RMID,0);
    return 0;
}
