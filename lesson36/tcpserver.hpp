#pragma once
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<sys/wait.h>
#include<pthread.h>
#include<signal.h>

#include "Task.hpp"
#include"ThreadPool.hpp"
#include "log.hpp"

namespace server
{
    using namespace std;

    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        ACCEPT_ERR
    };


    static const int gbacklog = 5;
    static const uint16_t gport = 8080;
    int gnum = 1024;

    class tcpserver;
    class ThreadData
    {
    public:
        ThreadData(tcpserver *self,int sock) 
        :_self(self),_sock(sock)
        {}
        ~ThreadData()=default;
    public:
        tcpserver *_self;
        int _sock;
    };


    class tcpserver
    {
    public:
        tcpserver(const uint16_t &port = gport)
            : _port(port), _listensock(-1)
        {
        }
        void initserver()
        {
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listensock < 0)
            {
                logMessage(FATAL, "socket error");
                exit(SOCKET_ERR);
            }
            logMessage(NORMAL, "create socket success. listen sock: %d",_listensock);
            
            struct sockaddr_in local;
            bzero(&local, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = INADDR_ANY;
            local.sin_port = htons(_port);
            
            if (bind(_listensock, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                logMessage(FATAL, "bind error");
                exit(BIND_ERR);
            }
            logMessage(NORMAL, "bind success");
            
            if (listen(_listensock, gbacklog) < 0)
            {
                logMessage(FATAL, "listen error");
            }
            logMessage(NORMAL, "listen success");
        }

        void start()
        {
            tao::ThreadPool<Task>::GetInstance()->run();
            logMessage(NORMAL,"Thread init success");
            // signal(SIGCHLD,SIG_IGN);
            while (true)
            {
                struct sockaddr_in peer;
                bzero(&peer, sizeof(peer));
                socklen_t len = sizeof(peer);
                int sock = accept(_listensock, (struct sockaddr *)&peer, &len);

                // accept不成功不用退出，accept下一个
                if (sock < 0)
                {
                    logMessage(ERROR, "accept error");
                    // exit(ACCEPT_ERR);
                    //!!!!!!11
                    continue;
                }
                logMessage(NORMAL, "accept a new link success. sock: %d",sock);
                //version 1 单进程单线程版
                // serviceIO(sock);
                // close(sock); // 一定要关

                //version 2 多进程版
                // pid_t id=fork();
                // if(id==0) //child
                // {
                //     close(_listensock);
                //     // if(fork()>0) exit(0);//非常巧妙
                //     serviceIO(sock);
                //     close(sock);
                //     exit(0);
                // }
                // // father;
                // close(sock);
                //可以选择忽略信号
                // pid_t ret=waitpid(id,nullptr,0);
                // if(ret>0)
                // {
                //     logMessage(NORMAL,"wait success: %d",ret);
                // }


                // version 3 多线程版
                // pthread_t tid;
                // ThreadData *td=new ThreadData(this,sock);
                // pthread_create(&tid,nullptr,threadroutine,td);

                // version 4 线程池

                // Task task(serviceIO,sock);
                tao::ThreadPool<Task>::GetInstance()->push(Task(serviceIO,sock));
            }
        }
        static void *threadroutine(void *args)
        {
            pthread_detach(pthread_self());
            ThreadData *td=static_cast<ThreadData*>(args);
            td->_self->serviceIO(td->_sock);
            close(td->_sock);
            delete td; 
            return nullptr;
        }
        static void serviceIO(int sock)
        {
            char buffer[gnum];
            while (true)
            {

                ssize_t n = read(sock, buffer, sizeof(buffer) - 1);
                if (n > 0)
                {
                    buffer[n] = 0;
                    // cout << "message# " << buffer << endl;
                    logMessage(NORMAL,"messagr# %s",buffer);
                    string response = buffer;
                    response += "[server echo]";
                    write(sock, response.c_str(), response.size());
                }
                else if (n == 0)
                {
                    logMessage(NORMAL, "client quit, me too");
                    break;
                }
            }
            close(sock);
        }
        ~tcpserver()
        {
            if (_listensock >= 0)
                close(_listensock);
        }

    private:
        uint16_t _port;
        int _listensock;
    };

}
