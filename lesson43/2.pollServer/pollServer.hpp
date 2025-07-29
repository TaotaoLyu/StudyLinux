#pragma once

#include <sys/select.h>
#include <sys/types.h>
#include <functional>
#include <sys/poll.h>
#include <string>
#include "sock.hpp"
#include "log.hpp"

namespace poll_ns
{
    static const int default_port = 8000;
    static const int pollfd_num = 2048;
    static const int buffer_num = 1024;
    static const int default_fd = -1;

    using func_t = std::function<std::string(const std::string &)>;

    class pollServer
    {
    public:
        pollServer(func_t func, uint16_t port = default_port) : _func(func), _listensock(-1), _port(port), _pollfds(nullptr)
        {
        }

        void initServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);

            _pollfds = new struct pollfd[pollfd_num];
            for (int i = 0; i < pollfd_num; ++i)
            {
                ResetItem(i);
            }

            _pollfds[0].fd = _listensock;
            _pollfds[0].events = POLLIN;
        }
        void Print()
        {
            std::cout << "fds: ";
            for (int i = 0; i < pollfd_num; ++i)
            {
                if (_pollfds[i].fd != default_fd)
                    std::cout << _pollfds[i].fd << " ";
            }
            std::cout << std::endl;
        }
        void Accepter()
        {
            std::string clientip;
            uint16_t clientport;
            int sock = Sock::Accept(_listensock, &clientip, &clientport);

            if (sock < 0)
                return;
            logMessage(NORMAL, "new link %s:%d", clientip.c_str(), clientport);

            // 拿到sock
            // 继续托管到poll
            int i = 0;
            for (; i < pollfd_num; ++i)
            {
                if (_pollfds[i].fd == default_fd)
                    break;
            }
            if (i == pollfd_num)
            {
                logMessage(NORMAL, " server load is too high!!!");
                close(sock);
            }
            else
            {

                _pollfds[i].fd = sock;
                _pollfds[i].events = POLLIN;
                _pollfds[i].revents = 0;
            }
            Print();
        }
        void ResetItem(int pos)
        {
            _pollfds[pos].fd = default_fd;
            _pollfds[pos].events = 0;
            _pollfds[pos].revents = 0;
        }
        void Recver(int pos)
        {
            int fd = _pollfds[pos].fd;
            char buffer[buffer_num];
            ssize_t n = recv(fd, buffer, sizeof(buffer) - 1,0);
            if (n > 0)
            {
                buffer[n] = 0;
                logMessage(NORMAL, "client# %s", buffer);
            }
            else if (n == 0)
            {
                close(fd);
                ResetItem(pos);
                logMessage(NORMAL,"client quit");
                return;
            }
            else
            {
                close(fd);
                ResetItem(pos);
                logMessage(ERROR,"client quit: %s",strerror(errno));
                return;
            }

            std::string reponse = _func(buffer);
            write(fd, reponse.c_str(), reponse.size());
        }
        void HandleEvent()
        {
            for (int i = 0; i < pollfd_num; ++i)
            {
                if (_pollfds[i].fd == default_fd)
                    continue;
                if (!(_pollfds[i].events & POLLIN))
                    continue;
                // 这里，fd肯定已经就绪
                if ((_pollfds[i].revents & POLLIN) && (_pollfds[i].fd == _listensock))
                {
                    Accepter();
                }
                else if (_pollfds[i].revents & POLLIN)
                {
                    Recver(i);
                }
                else 
                {}
                // logMessage(DEBUG,"i %d",i);
            }
        }
        void start()
        {

            int timeout = -5; // ms
            // 0 noblock
            //<0 block
            //>0 block in the time
            for (;;)
            {
                int n = poll(_pollfds, pollfd_num, timeout);
                if (n < 0)
                {
                    // error
                    logMessage(FATAL, "poll error errno %d(%s)",errno,strerror(errno));
                    exit(SELECT_ERR);
                }
                else if (n == 0)
                {
                    // timeout
                    logMessage(NORMAL, "time out ...");
                }
                else
                {
                    // new event
                    logMessage(NORMAL, "have event ready");
                    HandleEvent();
                }
            }
        }

        ~pollServer()
        {
            if (_listensock >= 0)
                close(_listensock);
            if (_pollfds)
                delete[] _pollfds;
        }

    private:
        int _listensock;
        uint16_t _port;
        struct pollfd *_pollfds;
        func_t _func;
    };

}
