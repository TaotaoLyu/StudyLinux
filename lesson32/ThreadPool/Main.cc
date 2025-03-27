#include<cstdio>
#include<unistd.h>
#include"ThreadPool.hpp"
#include "Task.hpp"


int main()
{
    // tao::ThreadPool<Task> tp;
    // tp.run();
    tao::ThreadPool<Task>::GetInstance()->run();
    while(true)
    {
        int x,y;
        char op;
        printf("input 1# ");
        std::cin>>x;
        printf("input 2# ");
        std::cin>>y;
        printf("input op# ");
        std::cin>>op;
        Task t(x,y,op,mymath);
        printf("Are you sure to submit? [y/n]");
        char confirm;
        std::cin>>confirm;
        if(confirm=='y')
            tao::ThreadPool<Task>::GetInstance()->push(t),usleep(50);
    }
    return 0;
}