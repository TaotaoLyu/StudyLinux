#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include"BlockQueue.hpp"
#include"Task.hpp"

using namespace std;

int cnt=1;
const std::string oper="+-*/%";


struct BlockQueues
{
    BlockQueue<CalTask> *c_bq;
    BlockQueue<SavTask> *s_bq;
};


void *produce(void *bq_)
{
    BlockQueue<CalTask> *bq=(static_cast<BlockQueues*>(bq_))->c_bq;
    while(true)
    {
        int x=rand()%50;
        int y=rand()%10;
        int opercode=rand()%oper.size();
        CalTask t(x,y,oper[opercode],mymath);
        bq->push(t);
        printf("生成任务 %s\n",t.ToString().c_str());
        sleep(1);

        
        // int data=cnt++;
        // bq->push(data);
        // cout<<"produce data: "<<data<<endl;
        // printf("produce data: %d\n",data);
    }

}
void *consume(void *bq_)
{
    // BlockQueue<int> *bq=static_cast<BlockQueue<int>*>(bq_);
    // BlockQueue<Task> *bq=static_cast<BlockQueue<Task>*>(bq_);
    BlockQueue<CalTask> *cbq=(static_cast<BlockQueues*>(bq_))->c_bq;
    BlockQueue<SavTask> *sbq=(static_cast<BlockQueues*>(bq_))->s_bq;
    while(true)
    {
        // int data;
        // bq->pop(&data);
        // cout<<"consume data: "<<data<<endl;
        CalTask t;
        cbq->pop(&t);
        std::string result=t();
        //const char* str=result.c_str();
        printf("处理完成 %s\n",result.c_str());
        // SavTask st(result,Save);
        // sbq->push(st);
        // printf("推送任务完成.....\n");
        // printf("consume data: %d\n",data);
        sleep(1);
    }
}

void *saving(void *bq_)
{
    BlockQueue<SavTask> *bq=(static_cast<BlockQueues*>(bq_))->s_bq;
    while(true)
    {
        SavTask t;
        bq->pop(&t);
        t();
        printf("完成存储任务...\n");
    }

}








int main()
{
    srand(time(0)^0x758432^rand());

    pthread_t producer,consumer,saver;
    // BlockQueue<int> *bq =new BlockQueue<int>();
    BlockQueue<CalTask> *c_bq =new BlockQueue<CalTask>();
    BlockQueue<SavTask> *s_bq =new BlockQueue<SavTask>();
    BlockQueues bqs;
    bqs.c_bq=c_bq;
    bqs.s_bq=s_bq;
    pthread_create(&producer,nullptr,produce,(void*)&bqs);
    // sleep(1);
    pthread_create(&consumer,nullptr,consume,(void*)&bqs);

    // pthread_create(&saver,nullptr,saving,(void*)&bqs);

    // pthread_t p[3];
    // pthread_t c[2];
    // pthread_create(p,nullptr,produce,(void*)&bqs);
    // pthread_create(p+1,nullptr,produce,(void*)&bqs);
    // pthread_create(p+2,nullptr,produce,(void*)&bqs);
    // pthread_create(c,nullptr,consume,(void*)&bqs);
    // pthread_create(c+1,nullptr,consume,(void*)&bqs);
    // pthread_join(p[0],nullptr);
    // pthread_join(p[1],nullptr);
    // pthread_join(p[2],nullptr);
    // pthread_join(c[0],nullptr);
    // pthread_join(c[1],nullptr);




    pthread_join(producer,nullptr);
    pthread_join(consumer,nullptr);
    pthread_join(saver,nullptr);

    delete c_bq;
    delete s_bq;

    return 0;
}