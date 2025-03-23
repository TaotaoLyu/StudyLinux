#pragma once
#include<vector>
#include<cassert>
#include<semaphore.h>
#include<pthread.h>


static const int gmax=5;

template<class T>
class RingQueue
{
    void P(sem_t &sem)
    {
        int n=sem_wait(&sem);
        assert(0==n);
        (void)n;
    }
    void V(sem_t &sem)
    {
        int n=sem_post(&sem);
        assert(0==n);
        (void)n;
    }

public:
    RingQueue(int capacity=gmax)
    :_queue(capacity),_capacity(capacity)
    {
        int n=sem_init(&_spacesem,0,_capacity);
        assert(0==n);
        n=sem_init(&_datasem,0,0);
        assert(0==n);
        pthread_mutex_init(&_pmutex,nullptr);
        pthread_mutex_init(&_cmutex,nullptr);
        _produceStep=_consumeStep=0;
    }
    void push(const T &in)
    {
        P(_spacesem);
        pthread_mutex_lock(&_pmutex);
        _queue[_produceStep++]=in;
        _produceStep%=_capacity;
        pthread_mutex_unlock(&_pmutex);
        V(_datasem);
    }
    void pop(T *out)
    {
        P(_datasem);
        pthread_mutex_lock(&_cmutex);
        *out=_queue[_consumeStep++];
        _consumeStep%=_capacity;
        pthread_mutex_unlock(&_cmutex);

        V(_spacesem);
    }

    ~RingQueue()
    {
        sem_destroy(&_spacesem);
        sem_destroy(&_datasem);
        pthread_mutex_destroy(&_pmutex);
        pthread_mutex_destroy(&_cmutex);
        _capacity=0;
    }

private:
    std::vector<T> _queue;
    int _capacity;
    sem_t _spacesem;
    sem_t _datasem;
    int _produceStep;
    int _consumeStep;
    pthread_mutex_t _pmutex;
    pthread_mutex_t _cmutex;
};