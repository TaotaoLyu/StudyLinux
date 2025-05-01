#pragma once
#include <functional>
#include <string>
#include <cstdio>
#include <iostream>

class Task
{
    using func_t = std::function<void(int)>;

public:
    Task()
    {
    }
    Task(func_t func,int sock)
        : _sock(sock),_callback(func)
    {
    }
    void operator()()
    {
        _callback(_sock);
    }
    ~Task() = default;

private:
    int _sock;
    func_t _callback;
};

