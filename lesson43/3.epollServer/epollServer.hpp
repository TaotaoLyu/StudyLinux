#pragma once

#include <sys/epoll.h>
#include "err.hpp"
#include "log.hpp"
#include "sock.hpp"

namespace epoll_ns
{
    static const int size = 128;
    static const int default_value=-1;
    class epollServer
    {
    public:
        epollServer(uint16_t port) : _listensock(-1), _port(port),_epfd(-1)
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
            ev.data.fd = _listensock;
            int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock, &ev);
            if (ret < 0)
            {
                logMessage(ERROR, "epoll_ctl error, errno %d(%s)", errno, strerror(errno));
            }
        }
        void start()
        {
            for (;;)
            {
                // epoll_wait();

                int n = 0;
                // int n=epoll_wait(_epfd,)
                switch (n)
                {
                case -1:
                    break;
                case 0:
                    break;
                default:
                    break;
                }
            }
        }
        ~epollServer()
        {
            if (_listensock > 0)
                close(_listensock);
            if(_epfd!=default_value)
                close(_epfd);
        }

    private:
        int _listensock;
        uint16_t _port;
        int _epfd;
    };

}