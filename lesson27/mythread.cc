#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<memory>
#include<unistd.h>
#include<pthread.h>
#include"Thread.hpp"
using namespace std;


__thread int g_val=1000;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *start_routine(void *arg)
{
    char *name=static_cast<char*>(arg);
    pthread_detach(pthread_self());
    while(true)
    {
        // cout<<"I am new thread!Runing...."<<" name: "<<name<<endl;
        pthread_t id=pthread_self();
        char buffer[64];
        snprintf(buffer,sizeof buffer,"0x%x",id);
        printf("I am new thread!Runing.... name: %s my_tid: %s g_val: %d &g_val: 0x%x\n",
            name,buffer,g_val,&g_val);
        g_val--;
        sleep(1);
    }
    return nullptr;
}

int tickets=10000;


void *get_tickets(void *args)
{
    char *name=static_cast<char*>(args);
    int cnt=0;

    while(true)
    {
        pthread_mutex_lock(&mutex);
        if(tickets>0)
        {
            usleep(1234);
            printf("%s is grabbing tickets. tickets: %d\n",name,tickets);
            tickets--;
            cnt++;
            // usleep(1234);
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

    }
    printf("%s:%d\n",name,cnt);
    return nullptr;
}



int main()
{

    unique_ptr<Thread> thread1(new Thread(get_tickets,(char*)"User 1",1));
    unique_ptr<Thread> thread2(new Thread(get_tickets,(char*)"User 2",2));
    unique_ptr<Thread> thread3(new Thread(get_tickets,(char*)"User 3",3));
    unique_ptr<Thread> thread4(new Thread(get_tickets,(char*)"User 4",4));

    thread1->start();
    thread2->start();
    thread3->start();
    thread4->start();

    thread1->join();
    thread2->join();
    thread3->join();
    thread4->join();
    


    // unique_ptr<Thread> thread1(new Thread(start_routine,(char*)"thread one",1));
    // thread1->start();

    // while(true)
    // {
    //     printf("I am main thread!Runing.... my_tid: 0x%x g_val: %d &g_val: 0x%x\n",
    //             pthread_self(),g_val,&g_val);
    //     sleep(1);
    // }

    

    // thread1->join();

    



    // pthread_t id;
    // pthread_create(&id,nullptr,start_routine,(char*)"thread one");
    // pthread_detach(id);
    // sleep(2);
    // // int n=pthread_join(id,nullptr);
    // // if(n!=0)
    // //     cout<<n<<":"<<strerror(n)<<endl;

    // while(true)
    // {
    //     printf("I am main thread!Runing.... my_tid: 0x%x newthread: 0x%x g_val: %d &g_val: 0x%x\n",
    //     pthread_self(),id,g_val,&g_val);
    //     sleep(1);
    // }
    // // pthread_join(id,nullptr);
    return 0;
}



