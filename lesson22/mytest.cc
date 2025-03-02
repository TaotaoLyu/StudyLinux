#include<iostream>
#include<cstdio>
#include<unistd.h>

int main()
{
    while(true)
    {
        printf("I am a process. pid: %d\n",getpid());
        sleep(1);
    }
    return 0;
}