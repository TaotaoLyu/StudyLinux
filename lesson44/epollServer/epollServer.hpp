#pragma once

#include <cassert>
#include <functional>
#include <string>
#include <sys/epoll.h>
#include "err.hpp"
#include "log.hpp"
#include "sock.hpp"

namespace epoll_ns
{
    static const int default_port = 8000;
    static const int size = 128;
    static const int default_value = -1;
    static const int default_num = 64;
    using func_t = std::function<std::string(const std::string &)>;
    class epollServer
    {
    public:
        epollServer(func_t func, uint16_t port = default_port, int num = default_num)
            : _listensock(default_value), _port(port), _epfd(default_value),
              _revs(nullptr), _num(num), _func(func)
        {
        }
        void initServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);

            _epfd = epoll_create(size); // 创建epoll模型
            if (_epfd < 0)
            {
                logMessage(FATAL, "epoll_create error, errno %d(%s)", errno, strerror(errno));
                exit(EPOLL_CREATE_ERR);
            }

            // 把listensock放进去
            struct epoll_event ev;
            memset(&ev, 0, sizeof(ev));
            ev.events = EPOLLIN;
            // ev.events = EPOLLIN|EPOLLET;
            ev.data.fd = _listensock;
            int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock, &ev);
            if (ret < 0)
            {
                logMessage(ERROR, "epoll_ctl error, errno %d(%s)", errno, strerror(errno));
            }

            _revs = new struct epoll_event[_num];

            logMessage(NORMAL, "init server success");
        }
        int Accepter()
        {
            std::string clientip;
            uint16_t clientport;

            int ret = Sock::Accept(_listensock, &clientip, &clientport);

            if (ret < 0)
                return -1;

            logMessage(NORMAL, "accept %s:%d", clientip.c_str(), clientport);
            return ret;
        }
        void HandleEvent(int event_num)
        {
            for (int i = 0; i < event_num; ++i)
            {
                uint32_t events = _revs[i].events;
                int sock = _revs[i].data.fd;

                if ((events & EPOLLIN) && sock == _listensock)
                {
                    int fd = Accepter();
                    if (fd <0)
                    {
                        logMessage(WARNING,"accept error");
                        continue;
                    }

                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    ev.data.fd = fd;
                    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
                    assert(ret == 0);
                }
                else if (events & EPOLLIN)
                {
                    char buffer[1024];
                    int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        logMessage(DEBUG, "client# %s", buffer);
                        std::string response = _func(buffer);
                        send(sock, response.c_str(), response.size(),0);
                    }
                    else if (n == 0)
                    {
                        // 先删除在close，ctl需要文件描述符是合法的
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr);
                        close(sock);
                        logMessage(NORMAL, "Client quit ...");
                    }
                    else
                    {
                        // 先删除在close，ctl需要文件描述符是合法的
                        epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr);
                        close(sock);
                        logMessage(ERROR, "recv error, errno %d(%s)",errno,strerror(errno));
                    }
                }
                else
                {
                }
            }
        }
        void start()
        {
            int timeout = -1; // ms
            for (;;)
            {
                // epoll_wait();
                int n = epoll_wait(_epfd, _revs, _num, timeout);
                switch (n)
                {
                case -1:
                    logMessage(ERROR, "epoll_wait error,errno %d(%s)", errno, strerror(errno));
                    break;
                case 0:
                    logMessage(NORMAL, "timeout ...");
                    break;
                default:
                    logMessage(NORMAL,"have event ready");
                    // HandleEvent(n);
                    break;
                }
            }
        }
        ~epollServer()
        {
            if (_listensock > 0)
                close(_listensock);
            if (_epfd != default_value)
                close(_epfd);

            if (_revs)
                delete[] _revs;
        }

    private:
        int _listensock;
        uint16_t _port;
        func_t _func;
        int _epfd;
        struct epoll_event *_revs;
        int _num;
    };

}