#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>


volatile int quit=0;

// void Count(int x)
// {
//     while(x--)
//     {
//         printf("cnt:%2d\r",x);
//         fflush(stdout);
//         sleep(1);
//     }
// }



void myhander(int signo)
{
    printf("pid: %d ,signal %d is captured\n",getpid(),signo);
    while (waitpid(-1,NULL,WNOHANG)>0)
    {
        
    }
    


    // printf("quit: %d -> ",quit);
    // quit=1;
    // printf("%d\n",quit);
}
int main()
{
    
    pid_t id=fork();
    // signal(17,myhander);
    // signal(SIGCHLD,SIG_IGN);
    if(id==0)
    {
        while(1)
        {
            sleep(1);
        }
    }
    //signal(SIGCHLD,SIG_IGN);
    signal(SIGCHLD,SIG_DFL);
    while(1)
    {
        sleep(1);
    }




    // signal(2,myhander);
    // while(!quit) 
    // {
    //     ;
    // }    
    // printf("Attention, I quit normally!\n");
    return 0;
}












// #include<iostream>
// #include<stdio.h>
// #include<signal.h>
// #include<unistd.h>


// using namespace std;


// void Count(int x)
// {
//     while(x--)
//     {
//         printf("cnt:%2d\r",x);
//         fflush(stdout);
//         sleep(1);
//     }
// }

// void myhander(int signo)
// {
//     cout<<"signal "<<signo<<" is captured!\n";
//     Count(20);
// }

// int main()
// {
//     struct sigaction act,oact;
//     act.sa_handler=myhander;
//     act.sa_flags=0;
    
//     sigemptyset(&act.sa_mask);
//     sigaddset(&act.sa_mask,2);
//     sigaction(3,&act,&oact);
//     //signal(3,myhander);
//     while(true) sleep(1);
//     return 0;
// }