#pragma once

#include<cerrno>
#include<cstdlib>
#include<cstring>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"log.hpp"
#include"err.hpp"


class Sock
{
    static const int default_backlog=32;
public:
    static int Socket()
    {
        int sock=socket(AF_INET,SOCK_STREAM,0);//注意
        if(sock<0)
        {
            logMessage(FATAL,"socket error errno %d(%s)",errno,strerror(errno));
            exit(SOCKET_ERR);
        }
        int opt=1;
        setsockopt(sock,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
        logMessage(NORMAL,"create new sock %d",sock);
        return sock;
    }
    static void Bind(int sock,uint16_t port)
    {
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family=AF_INET;
        local.sin_addr.s_addr=INADDR_ANY;
        local.sin_port=htons(port);
        if(bind(sock,(const sockaddr*)&local,sizeof(local))<0)
        {
            logMessage(FATAL,"bind error, errno %d(%s)",errno,strerror(errno));
            exit(BIND_ERR);
        }
        logMessage(NORMAL,"bind success ");
    }
    static void Listen(int sock)
    {
        if(listen(sock,default_backlog)<0)
        {
            logMessage(FATAL,"listen error, errno %d(%s)",errno,strerror(errno));
            exit(LISTEN_ERR);
        }
        logMessage(NORMAL,"listen socket success");

    }
    static int Accept(int listensock,std::string *clienip,uint16_t *clientport)
    {

        struct sockaddr_in peer;
        memset(&peer,0,sizeof(peer));
        socklen_t len=sizeof(peer);
        int sock=accept(listensock,(sockaddr*)&peer,&len);
        if(sock<0)
        {
            logMessage(ERROR,"accept error, errno %d(%s)",errno,strerror(errno));
            return -1;
        }
        else
        {
            logMessage(NORMAL,"accept new link,sock %d",sock);
            *clienip=inet_ntoa(peer.sin_addr);
            *clientport=ntohs(peer.sin_port);
            return sock;
        }

    }
};