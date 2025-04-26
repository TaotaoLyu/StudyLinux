#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

namespace Client
{
    using namespace std;
    class udpClient
    {
    public:
        udpClient(const string &serverip, const uint16_t &serverport)
            : _serverip(serverip), _serverport(serverport), _sockfd(-1), _quit(false)
        {
        }
        void initClient()
        {
            _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (_sockfd == -1)
            {
                cerr << "socket error: " << errno << " : " << strerror(errno) << endl;
                exit(2);
            }
            cout << "socket sucess. sockfd: " << _sockfd << endl;
            // 不需要显示地bind
        }
        static void *Recv(void *args)
        {
            int sockfd = *static_cast<int *>(args);
            pthread_detach(pthread_self());
            char buffer[2048];
            while(true)
            {
                struct sockaddr_in tmp;
                socklen_t len = sizeof(tmp);
                ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&tmp, &len);
                if (n >= 0)
                    buffer[n] = 0;
                printf("%s\n",buffer);
                fflush(stdout);

            }
            return nullptr;
        }
        void run()
        {
            pthread_create(&_Recv,NULL,Recv,&_sockfd);

            struct sockaddr_in server;
            bzero(&server, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(_serverip.c_str());
            server.sin_port = htons(_serverport); // 注意字节序(大小端)
            string message;
            while (!_quit)
            {
                // cout<<"Please Enter# ";
                cout << "Please Enter Cmd# ";
                // cerr << "Please Enter Message# ";
                getline(cin, message);
                // 第一次调用sendto时，OS自动bind
                ssize_t n = sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, sizeof(server));
                usleep(10000);
                // char buffer[2048];
                // struct sockaddr_in tmp;
                // socklen_t len=sizeof(tmp);
                // n=recvfrom(_sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&tmp,&len);
                // if(n>=0)
                //     buffer[n]=0;
                // cout<<buffer;
                // cout<<"Reply Answer# "<<buffer<<endl;
            }
        }
        ~udpClient() = default;

    private:
        string _serverip;
        uint16_t _serverport;
        int _sockfd;
        bool _quit;
        pthread_t _Recv;
    };
}