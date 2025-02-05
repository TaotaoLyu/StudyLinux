#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

#define NUM 10

typedef void(*fun_t)();

fun_t handlertask[NUM];

void task1()
{
    printf("task1.....\n");
}
void task2()
{
    printf("task2.....\n");
}
void task3()
{
    printf("task3.....\n");
}

void loadtask()
{
    handlertask[0]=task1;
    handlertask[1]=task2;
    handlertask[2]=task3;
}




int main()
{
    pid_t id=fork();
    if(id==0)
    {
        int cnt=10;
        while(cnt)
        {
            printf("i am child process, pid: %d , ppid: %d , cnt: %d\n",getpid(),getppid(),cnt--);
            sleep(1);
        }
        exit(66);
    }
    int status=0;
    //非阻塞等待
    loadtask();
    while(1)
    {
        pid_t ret=waitpid(id,&status,WNOHANG);
        if(ret>0)
        {
            printf("wait success!\n");
            if(WIFEXITED(status))
                printf("exit normal, child exit code: %d\n",WEXITSTATUS(status));
            else  
                printf("exit unexpectedly, sig num: %d\n",status&0x7F);
            return 0;
        }
        else if(ret==0) 
        {
            printf("wait done!!!child process is running....\n");
            printf("run other things...\n");
            int i=0;
            for(i=0;i<3;++i)
                handlertask[i]();
        }
        else 
        {
            printf("wait fail!!\n");
        }
        sleep(3);
    }



    //阻塞等待
    /*pid_t ret=waitpid(id,&status,0);
    if(ret>0)
    {
        printf("wait success!\n");
        if(WIFEXITED(status))
            printf("exit normal, child exit code: %d\n",WEXITSTATUS(status));
        else  
            printf("exit unexpectedly, sig num: %d\n",status&0x7F);
    }*/






    return 0;
}
