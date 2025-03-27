#pragma once

#include <string>
#include <cstdio>
#include <functional>
#include <cassert>
#include <pthread.h>

namespace ThreadNs
{
    class Thread
    {
        using func_t = std::function<void *(void *)>;
        static void *start_routine(void *args)
        {
            Thread *this_ = static_cast<Thread *>(args);
            return this_->func_(this_->args_);
        }

    public:
        Thread()
        {
            char buffer[64];
            snprintf(buffer, sizeof buffer, "thread:%d", threadnum++);
            name_ = buffer;
        }
        void start(func_t func, void *args)
        {
            func_ = func;
            args_ = args;
            int n = pthread_create(&tid_, nullptr, start_routine, this);
            assert(0 == n);
            (void)n;
        }

        void join()
        {
            int n = pthread_join(tid_, nullptr);
            assert(0 == n);
            (void)n;
        }
        std::string threadname()
        {
            return name_;
        }

        ~Thread()
        {
            // do nothing
        }

    private:
        pthread_t tid_;
        std::string name_;
        static int threadnum;
        func_t func_;
        void *args_;
    };
    int Thread::threadnum = 1;
}