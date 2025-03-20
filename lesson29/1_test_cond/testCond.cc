#include<iostream>
#include<cstdio>
#include<string>
#include<pthread.h>
#include<unistd.h>

using namespace std;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

int tickets=1000;

void *start_routine(void *arg)
{
    string name=static_cast<const char*>(arg);
    while(true)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        printf("%s -> %d\n",name.c_str(),tickets);
        tickets--;
        pthread_mutex_unlock(&mutex);
    }
}




int main()
{
    // pthread_t t1,t2;
    // pthread_create(&t1,nullptr,start_routine,(void*)"thread 1");
    // sleep(1);
    // pthread_create(&t2,nullptr,start_routine,(void*)"thread 2");
    // sleep(1);

    pthread_t t[5];
    for(int i=0;i<5;++i)
    {
        char *buffer=new char[64];
        snprintf(buffer,64,"thread %d",i+1);
        pthread_create(t+i,nullptr,start_routine,(void*)buffer);
        sleep(1);
    }

    while(true)
    {
        // pthread_cond_signal(&cond);
        pthread_cond_broadcast(&cond);
        printf("main thread wakeup one thread...\n");
        sleep(1);
    }



    for(int i=0;i<5;++i)
    pthread_join(t[i],nullptr);


    return 0;
}