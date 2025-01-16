#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t id=fork();
    if(id==0)
    {
        while(1)
        {
             printf("子进程,pid:%d ppid:%d\n",getpid(),getppid());
             sleep(2);
        }
    }
    else if(id>0)
    {
        while(1)
        {
             printf("父进程,pid:%d ppid:%d\n",getpid(),getppid());
             sleep(2);
        }
    }
    else 
    {
        //异常
    }
    /*while(1)
    {
        printf("我是一个进程 pid:%d\n",getpid());
        sleep(1);
    }*/
    return 0;
}
