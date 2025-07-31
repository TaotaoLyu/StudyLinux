#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <unistd.h>
#include <fcntl.h>
#include <binders.h>
#include "Err.hpp"
#include "Log.hpp"
#include "Socker.hpp"
#include "Epoller.hpp"
#include "Util.hpp"

namespace tcpserver
{

    class TcpServer;
    class Connection;

    static const uint16_t default_port = 8080;

    using func_t = std::function<void(Connection *)>;

    static const int default_fd = -1;
    class Connection
    {
    public:
        Connection(int sock) : sock_(sock), tsp_(nullptr)
        {
        }
        ~Connection()
        {
        }
        void Register(func_t recver, func_t sender, func_t exceper)
        {
            recver_ = recver;
            sender_ = sender;
            exceper_ = exceper;
        }

    public:
        int sock_;

        std::string inbuffer_;  // 输入缓冲区
        std::string outbuffer_; // 输出缓冲区

        func_t recver_;  // 处理读
        func_t sender_;  // 处理写
        func_t exceper_; // 处理异常事件

        TcpServer *tsp_; // 回指指针
    };
    class TcpServer
    {
    public:
        TcpServer(uint16_t port = default_port) : port_(port)
        {
        }
        void initServer()
        {
            //创建socket
            socker_.Create_Socket();
            socker_.Bind(port_);
            socker_.Listen();

            //创建epoll
            epoller_.Create();

            // 将目前唯一的一个sock，添加到epoller中， 之前需要先将对应的fd设置成为非阻塞
            // listensock_也是一个socket啊，也要看做成为一个Connection
            AddConnection(socker_.Get_Listensock(), EPOLLIN | EPOLLET, std::bind(&TcpServer::Accepter, this, std::placeholders::_1), nullptr, nullptr);
        }
        void Accepter(Connection *conn)
        {
            std::string clientip;
            uint16_t clienport;
            int sock = socker_.Accept(&clientip, &clienport);
            if (sock < 0)
                return;

            AddConnection(sock, EPOLLIN | EPOLLET, nullptr, nullptr, nullptr);
        }

        void AddConnection(int sock, uint32_t events, func_t recver, func_t sender, func_t exceper)
        {
            // 设置为非阻塞
            // 如果需要
            if (events & EPOLLET)
                Util::SetNonBlock(sock);

            // 创建connection
            Connection *conn = new Connection(sock);
            // 注册对应的回调处理方法
            conn->Register(recver, sender, exceper);

            // 添加epoll模型
            bool r=epoller_.AddEvent(sock, events);
            assert(r);
            (void)r;

            // 管理起来
            connections_.insert(std::pair<int, Connection *>(sock, conn));
        }

        void Dispatch()
        {
            for (;;)
            {
                // int n=epoll_wait();
            }
        }

        ~TcpServer()
        {
        }

    private:
        uint16_t port_;
        Socker socker_;
        Epoller epoller_;
        std::unordered_map<int, Connection *> connections_;
    };

}