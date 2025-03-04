#include<iostream>
#include<cstdio>

#include<signal.h>
#include<unistd.h>

#define BLOCK_SIGNAL 2
#define MAX_SIGNUM 31
using namespace std;

static void myhandler(int signo)
{
    printf("signal %d is delivered!!\n",signo);
}



static void show_pending(sigset_t &pending)
{
    for(int i=MAX_SIGNUM;i>=1;--i)
    {
        if(sigismember(&pending,i))
            cout<<1;
        else
            cout<<0;
    }
    cout<<endl;
}

int main()
{
    signal(2,myhandler);
    sigset_t block,oblock,pending;;
    //intit
    sigemptyset(&block);
    sigemptyset(&oblock);
    //block 2 signal
    sigaddset(&block,BLOCK_SIGNAL);
    //change kernel blocking bitset
    sigprocmask(SIG_SETMASK,&block,&oblock);
    int cnt=10;
    while(true)
    {
        sigemptyset(&pending);
        sigpending(&pending);
        show_pending(pending);
        sleep(1);
        if(cnt--==0)
        {
            cout<<"no blocking signal 2\n";
            sigprocmask(SIG_SETMASK,&oblock,&block);

        }
    }


    return 0;
}