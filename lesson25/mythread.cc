#include<iostream>
#include<cstdio>
#include<cassert>
#include<pthread.h>
#include<unistd.h>
using namespace std;

int g_val=0;

char* fun()
{
    return (char*)"I am a independent approach.";
}



void *thread_routine(void *arg)
{
    while(true)
    {
        printf("I am a new thread. arg: %s g_val: %d &gval: 0x%x fun: %s\n",
            arg,g_val++,&g_val,fun());
        sleep(1);
    }
}


int main()
{
    pthread_t tid;
    int n=pthread_create(&tid,nullptr,thread_routine,(void*)"thread one");
    assert(0==n);

    while(true)
    {
        printf("I am a main thread. g_val: %d &gval: 0x%x tid: 0x%x\n",
            g_val,&g_val,tid);
        sleep(1);
    }
    return 0;
}