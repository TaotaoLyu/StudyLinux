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

    using func_t = std::function<bool(const Request &, Response &)>;

    // 保证解耦
    void handlerEntry(int sock, func_t func)
    {
        std::string inbuffer;
        while (true)
        {
            // 获取完整的数据,正确获取序列化的数据，不多读，不少读
            // 不多读，不少读
            std::string recv_package, req_str;
            if (!recvPackage(sock, inbuffer, &recv_package)) // 堵塞式获取，写端关闭，return false
                return;

            if (!deLength(recv_package, &req_str))
                return;

            cout << "不带报头的:\n"
                 << req_str << endl;
            // 反序列，获得结构化的数据
            Request req;
            if (!req.deserialize(req_str))
                return;
            // 业务处理，解耦
            Response resp;
            func(req, resp);

            // 序列化数据
            std::string resp_str;
            if (!resp.serialize(&resp_str))
                return;
            cout << "不带报头的结果:\n"
                 << resp_str << endl;
            // 发送序列化数据
            std::string send_package = enLength(resp_str);
            cout << "带报头的报文:\n"
                 << send_package << endl;
            send(sock, send_package.c_str(), send_package.size(), 0);
        }
    }

    class calServer
    {
    public:
        calServer(const uint16_t &port = gport)
            : _port(port), _listensock(-1)
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
        // func 业务逻辑函数
        void start(func_t func)
        {
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
                    continue;
                }
                logMessage(NORMAL, "accept a new link success. sock: %d", sock);

                pid_t id = fork();
                if (id == 0) // child
                {
                    close(_listensock);
                    if (fork() > 0)
                        exit(0); // 非常巧妙
                    handlerEntry(sock, func);
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

        ~calServer()
        {
            if (_listensock >= 0)
                close(_listensock);
        }

    private:
        uint16_t _port;
        int _listensock;
    };

}
