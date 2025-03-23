#include<cstdio>

#include<pthread.h>
#include<unistd.h>
#include"RingQueue.hpp"
#include"Task.hpp"

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;


int cnt=1;
void *produce_routine(void *rq)
{
    RingQueue<Task> *ringqueue=static_cast<RingQueue<Task>*>(rq);
    while(true)
    {
        // version1
        // int data=cnt++;
        // ringqueue->push(data);
        // printf("produce data: %d\n",data);
        int x=rand()%50;
        int y=rand()%2;
        Task t(x,y,oper[rand()%oper.size()],mymath);
        ringqueue->push(t);
        printf("produce math task: %s\n",t.ToString().c_str());

        // sleep(1);
    }
}
void *consume_routine(void *rq)
{
    RingQueue<Task> *ringqueue=static_cast<RingQueue<Task>*>(rq);
    while(true)
    {
        // version1
        // int data;
        // ringqueue->pop(&data);
        // printf("consume data: %d\n",data);
        // sleep(1);
        Task t;
        ringqueue->pop(&t);
        std::string result=t();
        printf("consume math task: %s\n",result.c_str());

        // sleep(1);
    }


}



int main()
{
    srand(time(nullptr)^0x857^pthread_self());
    pthread_t p[3],c[6];
    RingQueue<Task> *rq=new RingQueue<Task>();
    for(int i=0;i<3;++i) pthread_create(p+i,nullptr,produce_routine,rq);
    for(int i=0;i<6;++i) pthread_create(c+i,nullptr,consume_routine,rq);

    for(int i=0;i<3;++i) pthread_join(p[i],nullptr);
    for(int i=0;i<6;++i) pthread_join(c[i],nullptr);


    // pthread_t p,c;
    // pthread_create(&p,nullptr,produce_routine,rq);
    // pthread_create(&p,nullptr,consume_routine,rq);

    // pthread_join(p,nullptr);
    // pthread_join(c,nullptr);
    return 0;
}