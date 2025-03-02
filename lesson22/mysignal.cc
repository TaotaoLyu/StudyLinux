#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>

using namespace std;

int cnt=0;

void catchSigal(int signo)
{
    int ret=alarm(0);
    printf("Remaining time: %d\n",ret);
    //printf("Capture signal ! signo: %d\n",signo);
    //printf("Capture signal ! signo: %d , cnt: %d\n",signo,cnt);
    //alarm(1);
    exit(2);
}

// ./mysignal pid signo
int main(int argc,char *argv[])
{
    signal(SIGALRM,catchSigal);
    alarm(60);
    while(true) ;
    //core dump
    // printf("pid: %d\n",getpid());
    // while(true)
    // {
    //     //sleep(1);
    //     int arr[10];
    //     arr[10000]=6666;
    // }



    // alarm (software conditions)
    // signal(SIGALRM,catchSigal);
    // alarm(1);
    // // int cnt=0;
    // while(true)
    // {
    //     //sleep(1);
    //     //printf("cnt: %d\n",cnt++);
    //     cnt++;
    // }



    // hardware
    // signal(11,catchSigal);
    // int *p=nullptr;
    // *p=100;
    // // signal(8,catchSigal);
    // // int a=10;
    // // a/=0;
    // while(true)
    // {
    //     sleep(1);
    //     // int a=10;
    //     // a/=0;
    //     // int *p=nullptr;
    //     // *p=100;
    // }




    // system interface
    // int cnt=0;
    // //signal(6,catchSigal);
    // while(true)
    // {
    //     sleep(1);
    //     printf("pid: %d cnt: %d\n",getpid(),cnt++);
    //     if(cnt==5)
    //         abort();
    //         //raise(3);
    // }


    // command
    // if(argc!=3)
    // {
    //     printf("Usage: ./mysignal [pid] [signo]\n");
    //     return 0;
    // }
    // pid_t pid=atoi(argv[1]);
    // int signo=atoi(argv[2]);
    // if(kill(pid,signo)==-1)
    //     perror("kill");


    // while(true)
    // {
    //     cout<<"I am a process. Running......\n";
    //     sleep(1);
    // }
    return 0;
}