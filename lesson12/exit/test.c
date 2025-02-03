#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    pid_t id=fork();
    if(id==0)
    {
        int cnt=5;
        while(cnt)
        {
            printf("我是子进程: %d 父进程: %d cnt: %d\n",getpid(),getppid(),cnt--);
            sleep(1);
        }
        int *p=NULL;
        *p=100;
        //int a=10;
       // a/=0;
        //sleep(5);
        exit(99);
    }
   // sleep(10);
    int status=0;
    pid_t ret=waitpid(id,&status,0);
    if(ret>0)
    {
        printf("wait success: %d ,sig number: %d ,child exit code: %d\nn",ret,status&(0x7F),(status>>8)&0xFF);
    }
   // sleep(5);
    return 66;
}
