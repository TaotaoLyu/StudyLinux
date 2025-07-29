#pragma once

#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "log.hpp"
#include "err.hpp"


class Sock
{
public:
    static int Socket()
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            logMessage(FATAL, "create sock error!!!");
            exit(SOCKET_ERR);
        }
        logMessage(NORMAL, "create new sock %d", sock);
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        return sock;
    }

    static void Bind(int sock, int port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(port);
        int ret = bind(sock, (const sockaddr *)&local, sizeof(local));

        if (ret < 0)
        {
            logMessage(FATAL, "bind error!!!");
            exit(BIND_ERR);
        }

        logMessage(NORMAL, "bind success!!!");
    }

    static void Listen(int sock, int backlog)
    {
        int ret = listen(sock, backlog);
        if (ret < 0)
        {
            logMessage(FATAL, "listen error!!!");
            exit(LISTEN_ERR);
        }

        logMessage(NORMAL, "listen success!!!");
    }
    static int Accept(int sock, std::string *clientip, uint16_t *port)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int ret = accept(sock, (sockaddr *)&peer, &len);

        if (ret < 0)
        {
            logMessage(ERROR, "accept error!!!");
        }
        else
        {
            *clientip = inet_ntoa(peer.sin_addr);
            *port = ntohs(peer.sin_port);

            logMessage(NORMAL, "accept new sock %d", ret);
        }
        return ret;
    }
};