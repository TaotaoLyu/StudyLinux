#include<iostream>
#include<thread>
#include<unistd.h>

using namespace std;


void thread_run()
{
    while(true)
    {
        cout<<"I am new thread...\n";
        sleep(1);
    }
}




int main()
{
    thread t1(thread_run);
    while(true)
    {
        cout<<"I am main thread...\n";
        sleep(1);
    }
    t1.join();
    return 0;
}




















// #include<iostream>
// #include<vector>
// #include<cstdio>
// #include<pthread.h>
// #include<unistd.h>
// #include<cstdlib>
// #include<cassert>

// using namespace std;

// #define NUM 5

// class ThreadData
// {
// public:
//     int num;
//     pthread_t id;
//     char buffer[64];
// };


// class ThreadReturn
// {
// public:
//     int exit_code;
//     int exit_result;
// };


// void *start_routine(void *arg)
// {
//     sleep(1);
//     ThreadData *td=static_cast<ThreadData*>(arg);
//     int cnt=10;
//     while(cnt)
//     {
//         // cout<<"cnt: "<<cnt<<" &cnt: "<<&cnt<<endl;
//         printf("cnt: %d &cnt: 0x%x\n",cnt,&cnt);
//         //printf("I am new thread! %s cnt: %d\n",td->buffer,cnt);
//         cnt--;
//         sleep(1);
//         // int *p=nullptr;
//         // *p=10;
//         //pthread_exit((void*)999);
//     }
//     //delete td;
//     // PTHREAD_CANCELED;

//     ThreadReturn *ret=new ThreadReturn;
//     ret->exit_code=66;
//     ret->exit_result=88;


//     return (void *)ret;
// }








// int main()
// {
//     //pthread_t id;
//     //pthread_create(&id,nullptr,start_routine,(void *)"thread one");
//     vector<ThreadData *> threads;
//     //create many thread
//     for(int i=0;i<NUM;++i)
//     {
//         ThreadData *td=new ThreadData;
//         td->num=i+1;
//         snprintf(td->buffer,sizeof(td->buffer),"%s: %d"," thread ",i+1);
//         pthread_create(&td->id,nullptr,start_routine,(void *)td);
//         threads.push_back(td);

//     }
//     for(auto &iter:threads)
//     {
//         printf("create thread %s : %uld success\n",iter->buffer,iter->id);
//     }
//     sleep(5);

//     for(int i=0;i<threads.size()/2;++i)
//     {
//         int n=pthread_cancel(threads[i]->id);
//         assert(n==0);
//         cout<<"pthread_cancel "<<threads[i]->buffer<<" success\n";
        
//     }


//     for(auto &iter:threads)
//     {

//         ThreadReturn *ret;
//         int n=pthread_join(iter->id,(void**)&ret);
//         assert(0==n);
//         printf("join %d success\n",iter->num);
//         //cout<<"join "<<iter->num<<" success! exit code: "<<ret->exit_code<<" exit result: "<<ret->exit_result<<endl;
//         delete iter;
//     }

//     cout<<"main thread quit\n";


//     // while(true)
//     // {
//     //     printf("I am main thread!\n");
//     //     sleep(1);
//     // }
//     return 0;
// }