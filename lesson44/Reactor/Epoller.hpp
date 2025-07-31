#pragma once

#include<cassert>
#include<cerrno>
#include<cstring>
#include<sys/epoll.h>
#include<unistd.h>
#include<fcntl.h>
#include"Err.hpp"
#include"Log.hpp"


class Epoller
{
    static const int default_fd=-1;
    static const int size=128;
public:
    Epoller():_epfd(default_fd)
    {

    }
    ~Epoller()
    {
        if(_epfd!=default_fd) close(_epfd);
    }
public:
    void Create()
    {
        _epfd=epoll_create(size);
        if(_epfd==-1)
        {
            logMessage(FATAL,"epoll_create error, errno %d(%s)",errno,strerror(errno));
            exit(EPOLL_CREATE_ERR);
        }
    }
    //user->kernel
    bool AddEvent(int sock,uint32_t events)
    {
        struct epoll_event ev;
        ev.data.fd=sock;
        ev.events=events;


        int ret=epoll_ctl(_epfd,EPOLL_CTL_ADD,sock,&ev);
        return ret==0;
    }


private:
    int _epfd;

};