#pragma once
#include <iostream>
#include <string>
#include<functional>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Server
{
    using namespace std;
    static const string defaultIp = "0.0.0.0";
    static const int gnum = 1024;
    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        OPEN_ERR
    };
    class udpServer
    {
        using func_t=function<void (int,string,uint16_t,string)>;
    public:
        udpServer(const func_t &callback,const uint16_t &port, const string &ip = defaultIp)
            : _callback(callback),_port(port), _ip(ip), _sockfd(-1)
        {
        }
        void initServer()
        {
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (_sockfd == -1)
            {
                cerr << "socket error: " << errno << " : " << strerror(errno) << endl;
                exit(SOCKET_ERR);
            }
            cout << "socket sucess. sockfd: " << _sockfd << endl;
            // bind
            struct sockaddr_in local;
            bzero(&local, sizeof(local)); // 填充字段置为零
            local.sin_family = AF_INET;
            //local.sin_addr.s_addr = inet_addr(_ip.c_str());//绑定所有IP,不指定IP
            local.sin_addr.s_addr=htonl(INADDR_ANY);
            local.sin_port = htons(_port);
            int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
            if (n == -1)
            {
                cerr << "bind error: " << errno << " : " << strerror(errno) << endl;
                exit(BIND_ERR);
            }
        }
        void start()
        {
            char buffer[gnum];
            for (;;)
            {
                struct sockaddr_in peer;
                bzero(&peer, sizeof(peer));
                socklen_t len = sizeof(peer);
                ssize_t n = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
                // 0表示阻塞式获取
                if (n >= 0)
                {
                    buffer[n] = '\0';
                    string clientip = inet_ntoa(peer.sin_addr);
                    uint16_t clientport = ntohs(peer.sin_port);
                    printf("%s[%d] message: %s\n", clientip.c_str(), clientport, buffer);
                    _callback(_sockfd,clientip,clientport,buffer);
                }

            }
        }
        ~udpServer()=default;

    private:
        string _ip;
        uint16_t _port;
        int _sockfd;
        func_t _callback;
    };

}