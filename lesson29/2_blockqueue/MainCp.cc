#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include"BlockQueue.hpp"

using namespace std;

int cnt=1;

void *produce(void *bq_)
{
    BlockQueue<int> *bq=static_cast<BlockQueue<int>*>(bq_);
    while(true)
    {
        int data=cnt++;
        bq->push(data);
        // cout<<"produce data: "<<data<<endl;
        printf("produce data: %d\n",data);

        sleep(1);
    }
}
void *consume(void *bq_)
{
    BlockQueue<int> *bq=static_cast<BlockQueue<int>*>(bq_);
    while(true)
    {
        int data;
        bq->pop(&data);
        // cout<<"consume data: "<<data<<endl;
        printf("consume data: %d\n",data);
        sleep(3);
    }
}








int main()
{
    pthread_t producer,consumer;
    BlockQueue<int> *bq =new BlockQueue<int>();
    pthread_create(&producer,nullptr,produce,(void*)bq);
    sleep(1);
    pthread_create(&consumer,nullptr,consume,(void*)bq);

    pthread_join(producer,nullptr);
    pthread_join(consumer,nullptr);


    return 0;
}