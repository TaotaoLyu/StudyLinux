#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    /*while(1)
    {
        printf("我是进程 pid: %d ppid: %d\n",getpid(),getppid());
        sleep(2);
    }*/
    //pid_t id=fork();
    /*if(id==0)
    {
        while(1)
        {
            printf("我是子进程 pid: %d ppid: %d\n",getpid(),getppid());
            sleep(1);
        }
        //exit(1);
    }
    else if(id>0)
    {
        while(1)
        {
             printf("我是父进程 pid: %d ppid: %d\n",getpid(),getppid());
             sleep(1);
        }
    }*/
    return 0;
}
