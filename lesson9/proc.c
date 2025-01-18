#include<stdio.h>
#include<unistd.h>

int main()
{
    int cnt=0;
    while(1)
    {
        printf("我是一个进程 %d\n",cnt++);
        sleep(2);
    }
    return 0;
}
