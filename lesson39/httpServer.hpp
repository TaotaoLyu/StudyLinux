#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>

#include "protocol.hpp"
#include "log.hpp"

namespace server
{
    using namespace std;

    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LISTEN_ERR,
        ACCEPT_ERR
    };

    static const int gbacklog = 5;
    static const uint16_t gport = 8080;
    int gnum = 1024;

    using func_t = std::function<bool(const HttpRequest &, HttpResponse &)>;

    class httpServer
    {
    public:
        httpServer(func_t func, const uint16_t &port = gport)
            : _func(func), _port(port), _listensock(-1)
        {
        }
        void initServer()
        {
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listensock < 0)
            {
                logMessage(FATAL, "socket error");
                exit(SOCKET_ERR);
            }
            logMessage(NORMAL, "create socket success. listen sock: %d", _listensock);

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
                exit(LISTEN_ERR);
            }
            logMessage(NORMAL, "listen success");
        }
        void HandlerHttp(int sock)
        {
            // 读取完整报文
            // 反序列化
            // 业务处理
            // 序列化
            // 发送报文

            char buffer[4096];
            ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                buffer[n] = 0;
                HttpRequest req;
                req.inbuffer = buffer;
                req.parase();
                HttpResponse resp;
                _func(req, resp);
                send(sock, resp.outbuffer.c_str(), resp.outbuffer.size(), 0);
            }

        }
        // func 业务逻辑函数
        void start()
        {
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
                    continue;
                }
                logMessage(NORMAL, "accept a new link success. sock: %d", sock);

                pid_t id = fork();
                if (id == 0) // child
                {
                    close(_listensock);
                    if (fork() > 0)
                        exit(0); // 非常巧妙
                    HandlerHttp(sock);
                    close(sock);
                    exit(0);
                }

                // father;
                close(sock);
                // 可以选择忽略信号
                pid_t ret = waitpid(id, nullptr, 0);
                if (ret > 0)
                {
                    logMessage(NORMAL, "wait success: %d", ret);
                }
            }
        }

        ~httpServer()
        {
            if (_listensock >= 0)
                close(_listensock);
        }

    private:
        uint16_t _port;
        int _listensock;
        func_t _func;
    };

}
