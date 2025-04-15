#pragma once
#include <iostream>
#include <string>
#include<cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Client
{
    using namespace std;
    class udpClient
    {
    public:
        udpClient(const string &serverip,const uint16_t &serverport)
        :_serverip(serverip),_serverport(serverport),_sockfd(-1),_quit(false)
        {}
        void initClient()
        {
            _sockfd=socket(AF_INET,SOCK_DGRAM,0);
            if (_sockfd == -1)
            {
                cerr << "socket error: " << errno << " : " << strerror(errno) << endl;
                exit(2);
            }
            cout<<"socket sucess. sockfd: "<<_sockfd<<endl;
            //不需要显示地bind
        }
        void run()
        {
            struct sockaddr_in server;
            bzero(&server,sizeof(server));
            server.sin_family=AF_INET;
            server.sin_addr.s_addr=inet_addr(_serverip.c_str());
            server.sin_port=htons(_serverport);//注意字节序(大小端)
            string message;
            while(!_quit)
            {
                cout<<"Please Enter# ";
                getline(cin,message);
                //第一次调用sendto时，OS自动bind
                ssize_t n=sendto(_sockfd,message.c_str(),message.size(),0,(struct sockaddr*)&server,sizeof(server));
            }
        }
        ~udpClient()=default;
    private:
        string _serverip;
        uint16_t _serverport;
        int _sockfd;
        bool _quit;
    };
}