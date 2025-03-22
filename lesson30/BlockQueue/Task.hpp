#pragma once
#include <functional>
#include <string>
#include <cstdio>
#include <iostream>

class CalTask
{
    using func_t = std::function<int(int, int, char)>;

public:
    CalTask()
    {
    }
    CalTask(int x, int y, char op,func_t func)
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
        int result = _func(_x, _y, _op);
        snprintf(buffer, sizeof buffer, "%d %c %d = ?", _x, _op, _y);
        return buffer;
    }


    ~CalTask() = default;

private:
    int _x;
    int _y;
    char _op;
    func_t _func;
};

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
            fprintf(stderr,"div zero error...\n");
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

class SavTask
{
    using func_t=std::function<void(const std::string &s)>;
public:
    SavTask()
    {}
    SavTask(const std::string &message,func_t func)
    :_message(message),_func(func)
    {}
    void operator()()
    {
        _func(_message);
    }
    ~SavTask()
    {}
private:
    std::string _message;
    func_t _func;
};

void Save(const std::string &message)
{
    FILE *fout=fopen("./log.txt","a+");
    if(!fout)
    {
        std::cerr<<"fopen error\n";
        return;
    }


    fputs(message.c_str(),fout);
    fputs("\n",fout);
    fclose(fout);
}