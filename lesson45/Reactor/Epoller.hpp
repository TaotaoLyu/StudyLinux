#pragma once

#include <cassert>
#include <cerrno>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include "Err.hpp"
#include "Log.hpp"

class Epoller
{
    static const int default_fd = -1;
    static const int size = 128;

public:
    Epoller() : _epfd(default_fd)
    {
    }
    ~Epoller()
    {
        if (_epfd != default_fd)
            close(_epfd);
    }

public:
    void Create()
    {
        _epfd = epoll_create(size);
        if (_epfd < 0)
        {
            logMessage(FATAL, "epoll_create error, errno %d(%s)", errno, strerror(errno));
            exit(EPOLL_CREATE_ERR);
        }
        logMessage(NORMAL, "epoll create success, epfd: %d", _epfd);
    }
    // user->kernel
    bool AddEvent(int sock, uint32_t events)
    {
        struct epoll_event ev;
        ev.data.fd = sock;
        ev.events = events;

        int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev);
        return ret == 0;
    }
    int Wait(struct epoll_event revs[], int num, int timeout)
    {
        return epoll_wait(_epfd, revs, num, timeout);
    }
    bool ModEvent(int sock, uint32_t event)
    {
        struct epoll_event ev;
        ev.data.fd = sock;
        ev.events =event;
        return epoll_ctl(_epfd, EPOLL_CTL_MOD, sock, &ev) == 0;
    }
    bool DelEvent(int sock)
    {
        return epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr) == 0;
    }

private:
    int _epfd;
};