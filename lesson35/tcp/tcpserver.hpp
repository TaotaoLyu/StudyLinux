#pragma once
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
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
            logMessage(NORMAL, "create socket success");

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
                    continue;
                }
                logMessage(NORMAL, "accept a new link success");
                cout << "sock: " << sock << endl;
                serviceIO(sock);
                close(sock); // 一定要关
            }
        }
        void serviceIO(int sock)
        {
            char buffer[gnum];
            while (true)
            {

                ssize_t n = read(sock, buffer, sizeof(buffer) - 1);
                if (n > 0)
                {
                    buffer[n] = 0;
                    cout << "message# " << buffer << endl;
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
