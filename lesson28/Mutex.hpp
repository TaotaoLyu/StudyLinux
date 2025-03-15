#ifndef _MUTEX_HPP_
#define _MUTEX_HPP_
// #pragma once
#include<pthread.h>
#include<cassert>

class Mutex
{
public:
    Mutex(pthread_mutex_t *mutex_p=nullptr):mutex_p_(mutex_p)
    {}
    void Lock()
    {
        if(mutex_p_==nullptr) return;
        int n=pthread_mutex_lock(mutex_p_);
        assert(0==n);
        (void)n;
    }
    void unLock()
    {
        if(mutex_p_==nullptr) return;
        int n=pthread_mutex_unlock(mutex_p_);
        assert(0==n);
        (void)n;
    }
    ~Mutex()=default;
private:
    pthread_mutex_t *mutex_p_;
};


class LockGuard
{
public:
    LockGuard(pthread_mutex_t *mutex_p):mutex(mutex_p)
    {
        mutex.Lock();
    }
    ~LockGuard()
    {
        mutex.unLock();
    }

private:
    Mutex mutex;
};










#endif