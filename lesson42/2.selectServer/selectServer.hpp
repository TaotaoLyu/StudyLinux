#pragma once

#include <sys/select.h>
#include <sys/types.h>
#include "sock.hpp"
#include "log.hpp"

namespace select_ns
{
    const int default_port = 8000;
    const int default_backlog = 10;
    const int fd_num = sizeof(fd_set) * 8;
    const int default_fd = -1;
    class selectServer
    {
    public:
        selectServer(int port = default_port) : _listensock(-1), _port(port), fdarray(nullptr)
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
        void HandleEvent(fd_set rfds)
        {
            if (FD_ISSET(_listensock, &rfds)) // 如果listensock的读准备就绪
            {

                std::string clientip;
                uint16_t clientport;
                int sock = Sock::Accept(_listensock, &clientip, &clientport);
                logMessage(NORMAL, "new link %s:%d", clientip.c_str(), clientport);

                if (sock < 0)
                    return;

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
                int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr);//注意maxfd

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
                    HandleEvent(rfds);
                    logMessage(NORMAL, "get a new link");
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
    };

}
