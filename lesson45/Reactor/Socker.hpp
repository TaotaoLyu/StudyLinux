#pragma once

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"
#include "Err.hpp"

class Socker
{
    static const int default_backlog = 32;
    static const int default_fd = -1;

public:
    Socker()
        : _listensock(default_fd)
    {
    }
    ~Socker()
    {
        if (_listensock != default_fd)
            close(_listensock);
    }

public:
    void Create_Socket()
    {
        _listensock = socket(AF_INET, SOCK_STREAM, 0); // 注意
        if (_listensock < 0)
        {
            logMessage(FATAL, "creat socket error errno %d(%s)", errno, strerror(errno));
            exit(SOCKET_ERR);
        }
        int opt = 1;
        setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        logMessage(NORMAL, "create new sock %d", _listensock);
    }
    void Bind(int port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(port);
        if (bind(_listensock, (const sockaddr *)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind error, errno %d(%s)", errno, strerror(errno));
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "bind success ");
    }
    void Listen()
    {
        if (listen(_listensock, default_backlog) < 0)
        {
            logMessage(FATAL, "listen error, errno %d(%s)", errno, strerror(errno));
            exit(LISTEN_ERR);
        }
        logMessage(NORMAL, "listen socket success");
    }
    int Accept(std::string *clienip, uint16_t *clientport,int *err)
    {

        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        socklen_t len = sizeof(peer);
        int sock = accept(_listensock, (sockaddr *)&peer, &len);
        *err=errno;
        if (sock < 0)
        {
            logMessage(ERROR, "accept error, errno %d(%s)", errno, strerror(errno));
            return -1;
        }
        else
        {
            logMessage(NORMAL, "accept new link,sock %d", sock);
            *clienip = inet_ntoa(peer.sin_addr);
            *clientport = ntohs(peer.sin_port);
            return sock;
        }
    }
    int Get_Listensock()
    {
        return _listensock;
    }

private:
    int _listensock;
};