#pragma once

#include<string>
#include<cstdio>
#include<functional>
#include<cassert>
#include<pthread.h>

class Thread;

// struct Context
// {
//     Thread *this_;
// };



class Thread
{
    using func_t=std::function<void*(void*)>;
    static void *start_routine(void *args)
    {
        Thread *this_=static_cast<Thread*>(args);
        return this_->func_(this_->args_);
        // Context *con=static_cast<Context*>(args);
        // func_(args);
        // con->this_->func_(con->args_);
        // delete con;
    }
public:
    Thread(func_t func,void *args,int num):func_(func),args_(args)
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer,"thread:%d",num);
        name_=buffer;
    }
    void start()
    {
        // Context *con=new Context;
        // con->this_=this;
        int n=pthread_create(&tid_,nullptr,start_routine,this);
        assert(0==n);
    }

    void join()
    {
        int n=pthread_join(tid_,nullptr);
        assert(0==n);
    }

    // void *run(void *args)
    // {
    //     return func_(args);
    // }

    ~Thread()
    {
        //do nothing
    }



private:
    pthread_t tid_;
    std::string name_;
    func_t func_;
    void *args_;
};