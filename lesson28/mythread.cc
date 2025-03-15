#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <memory>
#include <unistd.h>
#include <pthread.h>
#include<vector>
#include "Thread.hpp"
#include "Mutex.hpp"
using namespace std;

__thread int g_val = 1000;

void *start_routine(void *arg)
{
    char *name = static_cast<char *>(arg);
    pthread_detach(pthread_self());
    while (true)
    {
        // cout<<"I am new thread!Runing...."<<" name: "<<name<<endl;
        pthread_t id = pthread_self();
        char buffer[64];
        snprintf(buffer, sizeof buffer, "0x%x", id);
        printf("I am new thread!Runing.... name: %s my_tid: %s g_val: %d &g_val: 0x%x\n",
               name, buffer, g_val, &g_val);
        g_val--;
        sleep(1);
    }
    return nullptr;
}

int tickets = 10000;

class ThreadData
{
public:
    ThreadData(std::string name, pthread_mutex_t *mutex_p) : name_(name), mutex_p_(mutex_p)
    {
    }
    ~ThreadData() = default;

public:
    pthread_mutex_t *mutex_p_;
    std::string name_;
};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *get_tickets(void *args)
{
    // char *name = static_cast<char *>(args);
    ThreadData *d = static_cast<ThreadData *>(args);
    int cnt = 0;

    while (true)
    {
        // pthread_mutex_lock(d->mutex_p_);
        // pthread_mutex_lock(&mutex);
        {
            LockGuard lockguard(d->mutex_p_);
            if (tickets > 0)
            {
                usleep(1234);
                // printf("%s is grabbing tickets. tickets: %d\n", name, tickets);
                printf("%s is grabbing tickets. tickets: %d\n", d->name_.c_str(), tickets);
                tickets--;
                cnt++;
                // usleep(1234);
                // pthread_mutex_unlock(&mutex);
                // pthread_mutex_unlock(d->mutex_p_);
            }
            else
            {
                // pthread_mutex_unlock(d->mutex_p_);
                // pthread_mutex_unlock(&mutex);
                break;
            }
            // usleep(1234);
        }
        usleep(1234);
    }
    // printf("%s:%d\n", name, cnt);
    printf("%s:%d\n", d->name_.c_str(), cnt);
    delete d;
    return nullptr;
}

int main()
{

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, nullptr);

#define NUM 4
    
    vector<pthread_t> threads(NUM);
    for(int i=0;i<NUM;++i)
    {
        char buffer[64];
        snprintf(buffer,sizeof(buffer),"User %d",i+1);
        ThreadData *d=new ThreadData(buffer,&lock);
        pthread_create(&threads[i],nullptr,get_tickets,d);
    }
    for(const auto &tid:threads)
        pthread_join(tid,nullptr);




    pthread_mutex_destroy(&lock);
    // pthread_t t1,t2,t3,t4;

    // pthread_create(&t1, nullptr, get_tickets, (void*)"User 1");
    // pthread_create(&t2, nullptr, get_tickets, (void*)"User 2");
    // pthread_create(&t3, nullptr, get_tickets, (void*)"User 3");
    // pthread_create(&t4, nullptr, get_tickets, (void*)"User 4");

    // pthread_join(t1, nullptr);
    // pthread_join(t2, nullptr);
    // pthread_join(t3, nullptr);
    // pthread_join(t4, nullptr);

    // unique_ptr<Thread> thread1(new Thread(get_tickets,(char*)"User 1",1));
    // unique_ptr<Thread> thread2(new Thread(get_tickets,(char*)"User 2",2));
    // unique_ptr<Thread> thread3(new Thread(get_tickets,(char*)"User 3",3));
    // unique_ptr<Thread> thread4(new Thread(get_tickets,(char*)"User 4",4));

    // thread1->start();
    // thread2->start();
    // thread3->start();
    // thread4->start();

    // thread1->join();
    // thread2->join();
    // thread3->join();
    // thread4->join();

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
