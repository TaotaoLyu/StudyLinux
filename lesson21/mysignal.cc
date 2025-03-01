#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<cstdlib>



void hander(int signo)
{
    std::cout<<"Capture signal. The signal is "<<signo<<std::endl;
    exit(66);
}

int main()
{
    signal(SIGINT,hander);   //register function
    while(true)
    {
        std::cout<<" I am a process. pid: "<<getpid()<<std::endl;
        sleep(1);
    }
    return 0;
}