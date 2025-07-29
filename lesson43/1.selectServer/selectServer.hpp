#pragma once

#include <sys/select.h>
#include <sys/types.h>
#include <functional>
#include <string>
#include "sock.hpp"
#include "log.hpp"

namespace select_ns
{
    static const int default_port = 8000;
    static const int default_backlog = 10;
    static const int fd_num = sizeof(fd_set) * 8;
    static const int buffer_num = 1024;
    static const int default_fd = -1;
    using func_t = std::function<std::string(const std::string &)>;
    class selectServer
    {
    public:
        selectServer(func_t f, int port = default_port) : func(f), _listensock(-1), _port(port), fdarray(nullptr)
        {
        }

        void initServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock, default_backlog);

            fdarray = new int[fd_num];
            for (int i = 0; i < fd_num; ++i)
                fdarray[i] = default_fd;

            fdarray[0] = _listensock;
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
            // 继续托管到select
            int i = 0;
            for (; i < fd_num; ++i)
            {
                if (fdarray[i] == default_fd)
                    break;
            }

            if (i == fd_num)
            {
                logMessage(NORMAL, " server load is too high!!!");
                close(sock);
            }
            else
            {

                fdarray[i] = sock;
            }
        }
        void Recver(int pos)
        {
            int fd = fdarray[pos];
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
                fdarray[pos] = default_fd;
                logMessage(NORMAL,"clent quit");
                return;
            }
            else
            {
                close(fd);
                fdarray[pos] = default_fd;
                logMessage(ERROR,"client quit: %s",strerror(errno));
                return;
            }
            std::string reponse = func(buffer);
            write(fd, reponse.c_str(), reponse.size());
        }
        void HandleEvent(fd_set rfds)
        {
            for (int i = 0; i < fd_num; ++i)
            {
                if (fdarray[i] == default_fd)
                    continue;
                if (!FD_ISSET(fdarray[i], &rfds))
                    continue;
                // 这里，fd肯定已经就绪
                if (fdarray[i] == _listensock)
                {
                    Accepter();
                }
                else
                {
                    Recver(i);
                }
            }
        }
        void start()
        {

            for (;;)
            {
                fd_set rfds;
                FD_ZERO(&rfds);
                // FD_SET(_listensock, &rfds);
                int maxfd = fdarray[0];
                for (int i = 0; i < fd_num; ++i)
                {
                    if (fdarray[i] == -1)
                        continue;
                    FD_SET(fdarray[i], &rfds);
                    if (maxfd < fdarray[i])
                        maxfd = fdarray[i];
                }

                // timeval t = {3, 0};

                // int n = select(maxfd + 1, &rfds, nullptr, nullptr, &t);
                int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr); // 注意maxfd

                if (n < 0)
                {
                    // error
                    logMessage(FATAL, "select error!!!");
                    exit(SELECT_ERR);
                }
                else if (n == 0)
                {
                    // timeout
                    logMessage(NORMAL, "time out ...");
                }
                else
                {
                    // new link
                    logMessage(NORMAL, "have event ready");
                    HandleEvent(rfds);
                }

                // 阻塞式获取连接
                //  std::string clientip;
                //  uint16_t clientport;
                //  int sock=Sock::Accept(_listensock,&clientip,&clientport);
                //  logMessage(NORMAL,"new link %s:%d",clientip.c_str(),clientport);
            }
        }

        ~selectServer()
        {
            if (_listensock >= 0)
                close(_listensock);
            if (fdarray)
                delete[] fdarray;
        }

    private:
        int _listensock;
        int _port;
        int *fdarray;
        func_t func;
    };

}
