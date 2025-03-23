#pragma once
#include <functional>
#include <string>
#include <cstdio>
#include <iostream>

class Task
{
    using func_t = std::function<int(int, int, char)>;

public:
    Task()
    {
    }
    Task(int x, int y, char op,func_t func)
        : _x(x), _y(y), _op(op),_func(func)
    {
    }
    std::string operator()()
    {
        char buffer[64];
        int result = _func(_x, _y, _op);
        snprintf(buffer, sizeof buffer, "%d %c %d = %d", _x, _op, _y, result);
        return buffer;
    }
    std::string ToString()
    {
        char buffer[64];
        snprintf(buffer, sizeof buffer, "%d %c %d = ?", _x, _op, _y);
        return buffer;
    }


    ~Task() = default;

private:
    int _x;
    int _y;
    char _op;
    func_t _func;
};

const std::string oper="+-*/%"; 

int mymath(int x, int y, char op)
{
    int result = 0;
    switch (op)
    {
    case '+':
        result = x + y;
        break;
    case '-':
        result = x - y;
        break;
    case '*':
        result = x * y;
        break;
    case '/':
    {
        if (y == 0)
        {
            // std::cerr << "div zero error..." << std::endl;
            // fprintf(stderr,"div zero error...\n");
            printf("div zero error...\n");
            result = -1;
        }
        else
            result = x / y;
    }
        break;
    case '%':
    {
        if (y == 0)
        {
            // std::cerr << "mod zero error..." << std::endl;
            fprintf(stderr,"mod zero error...\n");
            result = -1;
        }
        else
            result = x % y;
    }
        break;
    default:
        break;
    }
    return result;
}
