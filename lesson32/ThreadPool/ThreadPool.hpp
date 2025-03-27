#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include "Thread.hpp"
#include "Mutex.hpp"

namespace tao
{
    using namespace ThreadNs;
    using namespace MutexNs;
    static int gmax = 5;
    template <class T>
    class ThreadPool;

    template <class T>
    struct ThreadData
    {
        ThreadPool<T> *_threadpool;
        std::string _name;
        ThreadData(ThreadPool<T> *t, const std::string &n) : _threadpool(t), _name(n) {}
    };

    template <class T>
    class ThreadPool
    {
    private:
        static void *handler(void *args)
        {
            ThreadData<T> *td = static_cast<ThreadData<T> *>(args);
            ThreadPool<T> *tp = td->_threadpool;
            while (true)
            {
                T t;
                {
                    LockGuard lockguard(&tp->_mutex);
                    // pthread_mutex_lock(&tp->_mutex);
                    while (tp->_task_queue.empty())
                    {
                        pthread_cond_wait(&tp->_cond, &tp->_mutex);
                    }
                    t = tp->_task_queue.front();
                    tp->_task_queue.pop();
                    // pthread_mutex_unlock(&tp->_mutex);
                }
                std::string result = t();
                printf("%s solve %s ,answer %s\n", td->_name.c_str(), t.ToString().c_str(), result.c_str());
            }
            delete td;
            return nullptr;
        }
        // ThreadPool(int num = gmax) : _num(num)
        ThreadPool(int num) : _num(num)
        {
            pthread_mutex_init(&_mutex, nullptr);
            pthread_cond_init(&_cond, nullptr);
            for (int i = 0; i < _num; ++i)
            {
                // _threads.push_back(new Thread(handler, this));
                _threads.push_back(new Thread());
            }
        }
        ThreadPool<T>(ThreadPool<T> &)=delete;
        ThreadPool<T>& operator=(const ThreadPool<T> &)=delete;

    public:
        static ThreadPool<T> *GetInstance(int num = gmax)
        {
            if (_tp == nullptr)
            {
                _singlock.lock();
                if (_tp == nullptr)
                    _tp = new ThreadPool(num);
                _singlock.unlock();
            }
            // return _tp;
            return const_cast<ThreadPool<T>*>(_tp);
        }
        void run() 
        {
            for (auto &t : _threads)
            {
                ThreadData<T> *td = new ThreadData<T>(this, t->threadname());
                t->start(handler, td);
                printf("%s run....\n",t->threadname().c_str());
            }
        }
        void push(const T &in) 
        {
            // pthread_mutex_lock(&_mutex);
            LockGuard lockguard(&_mutex);
            _task_queue.push(in);
            pthread_cond_signal(&_cond);
            // pthread_mutex_unlock(&_mutex);
        }
        ~ThreadPool()
        {
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_cond);
            for (auto &t : _threads)
                t->join(),delete t;
        }

    private:
        int _num;
        std::queue<T> _task_queue;
        std::vector<Thread *> _threads;
        pthread_mutex_t _mutex;
        pthread_cond_t _cond;
        volatile static ThreadPool<T> *_tp;
        static std::mutex _singlock;
    };
    template <class T>
    volatile ThreadPool<T> *ThreadPool<T>::_tp = nullptr;

    template<class T>
    std::mutex ThreadPool<T>::_singlock;

}