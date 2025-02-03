#include<stdio.h>
#include<unistd.h>

int global_val=100;
int main()
{
    pid_t id=fork();
    if(id<0)
        printf("error\n");
    else if(id==0)
    {
        //子进程
        int cnt=0;
        while(1)
        {
            printf("我是子进程 pid: %d ppid: %d global_val: %d &global_val: %p\n",getpid(),getppid(),global_val,&global_val);
            sleep(1);
            cnt++;
            if(cnt==10)
            {
                global_val=300;
                printf("global_val已修改\n");
            }
        }
    }
    else 
    {
        while(1)
        {
            printf("我是父进程 pid: %d ppid: %d global_val: %d &global_val: %p\n",getpid(),getppid(),global_val,&global_val);
            sleep(1);
        }   
    }



    return 0;
}
