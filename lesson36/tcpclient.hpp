#pragma once
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace client
{
    using namespace std;

    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        CONNECT_ERR,
    };
    int gnum = 1024;
    class tcpclient
    {
    public:
        tcpclient(const string &serverip, const uint16_t &serverport)
            : _serverip(serverip), _serverport(serverport), _sock(-1)
        {
        }
        void initclient()
        {
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_sock < 0)
            {
                cerr << "socket create error" << endl;
                exit(SOCKET_ERR);
            }
        }
        void start()
        {
            struct sockaddr_in server;
            bzero(&server, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(_serverip.c_str());
            server.sin_port = htons(_serverport);
            //连接失败不用退出
            //连接下一个
            if (connect(_sock, (struct sockaddr *)&server, sizeof(server)) != 0)
            {
                cerr << "socket connect error" << endl;
            }
            else
            {
                string msg;
                while (true)
                {
                    cout << "Please Enter# ";
                    getline(cin, msg);
                    write(_sock, msg.c_str(), msg.size());

                    char buffer[gnum];
                    ssize_t n = read(_sock, buffer, sizeof(buffer) - 1);
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        cout << "Server Reply# " << buffer << endl;
                    }
                    else if (n == 0)
                    {
                        cout << "server quit. me too" << endl;
                        break;
                    }
                }
            }
        }
        ~tcpclient()
        {
            if (_sock >= 0)
                close(_sock);
        }

    private:
        string _serverip;
        uint16_t _serverport;
        int _sock;
    };

}
