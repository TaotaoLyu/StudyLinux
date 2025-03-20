#ifndef _BLOCKQUEUE_HPP_
#define _BLOCKQUEUE_HPP_

#include<queue>
#include<pthread.h>





template<class T>
class BlockQueue
{
    static int gmax;
public:
    BlockQueue(int maxcap=gmax):_maxcap(maxcap)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_pcond,nullptr);
        pthread_cond_init(&_ccond,nullptr);
    }
    void push(const T &data)
    {
        pthread_mutex_lock(&_mutex);
        // if(is_full())
        while(is_full())//细节
        {
            pthread_cond_wait(&_pcond,&_mutex);
        }
        _q.push(data);
        pthread_cond_signal(&_ccond);
        pthread_mutex_unlock(&_mutex);
    }
    void pop(T *data)
    {
        pthread_mutex_lock(&_mutex);
        // if(is_empty())
        while(is_empty())
        {
            pthread_cond_wait(&_ccond,&_mutex);
        }
        *data=_q.front();
        _q.pop();
        pthread_cond_signal(&_pcond);
        pthread_mutex_unlock(&_mutex);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_pcond);
        pthread_cond_destroy(&_ccond);
    }
private:
    bool is_full()
    {
        return _q.size()==_maxcap;
    }
    bool is_empty()
    {
        return _q.empty();
    }

private:
    std::queue<T> _q;
    int _maxcap;
    pthread_mutex_t _mutex;
    pthread_cond_t _pcond;
    pthread_cond_t _ccond;
};

template<class T>
int BlockQueue<T>::gmax=15;


#endif