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
    static const int num = 128;

    using func_t = std::function<void(Connection *)>;

    static const int default_fd = -1;
    class Connection
    {
    public:
        Connection(int sock,TcpServer *tsp) : sock_(sock), tsp_(tsp)
        {
            recver_ = nullptr;
            sender_ = nullptr;
            excepter_ = nullptr;
        }
        void Register(func_t recver, func_t sender, func_t excepter)
        {
            recver_ = recver;
            sender_ = sender;
            excepter_ = excepter;
        }
        ~Connection() = default;
        void Close()
        {
            if (sock_ >= 0)
                close(sock_);
        }

    public:
        int sock_;

        std::string inbuffer_;  // 输入缓冲区
        std::string outbuffer_; // 输出缓冲区

        func_t recver_;   // 处理读
        func_t sender_;   // 处理写
        func_t excepter_; // 处理异常事件

        TcpServer *tsp_; // 回指指针

        uint64_t last_time; // 保存最近访问时间，来进行连接管理，5分钟不操作直接关掉
    };
    class TcpServer
    {
    private:
        void Recver(Connection *conn)
        {
            // conn->last_time=time(nullptr);
            char buffer[1024];
            while (true)
            {
                ssize_t n = recv(conn->sock_, buffer, sizeof(buffer) - 1, 0); // 非阻塞地读
                if (n > 0)
                {
                    buffer[n] = 0;
                    conn->inbuffer_ += buffer;
                    // 业务处理
                    service_(conn);
                }
                else if (n == 0)
                {
                    // 关闭了
                    if (conn->excepter_)
                    {
                        conn->excepter_(conn);
                        return;
                    }
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) // 数据未就绪
                    {
                        return; // 下次再读
                    }
                    else if (errno == EINTR) // 系统中断而读取失败
                    {
                        continue; // 继续读
                    }
                    else
                    {
                        // 真的错了
                        if (conn->excepter_)
                        {
                            conn->excepter_(conn);
                            return;
                        }
                    }
                }
            }
        }
        void Sender(Connection *conn)
        {
            // conn->last_time=time(nullptr);

            while (true)
            {
                // 可能会有SIGPIP 在守护进程中处理

                ssize_t n = send(conn->sock_, conn->outbuffer_.c_str(), conn->outbuffer_.size(), 0);
                if (n > 0)
                {
                    conn->outbuffer_.erase(0, n);
                    if(conn->outbuffer_.empty()) break;
                }
                else
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) // 空间未就绪
                    {
                        break;
                    }
                    else if (errno == EINTR) // 系统中断而发送失败
                    {
                        continue; // 继续发送
                    }
                    else
                    {
                        // 真的错了
                        if (conn->excepter_)
                        {
                            conn->excepter_(conn);
                            return;
                        }
                    }
                }
            }

            //是否添加读事件关心
            if (!conn->outbuffer_.empty()) // 不为空
                EnablieReadWrite(conn,true,true);
            else // 发完了
                EnablieReadWrite(conn,true,false);
        }
        void Excepter(Connection *conn)
        {
            // 异常直接关掉
            // 从epoll模型中去除
            epoller_.DelEvent(conn->sock_);

            // 关闭套接字
            conn->Close();

            // 从map中移除
            connections_.erase(conn->sock_);

            logMessage(NORMAL, "close sock %d", conn->sock_);
            // 释放内存
            delete conn;
        }
        void Accepter(Connection *conn)
        {
            // 循环读取，因为只会通知一次
            for (;;)
            {
                std::string clientip;
                uint16_t clienport;
                int err = 0;
                int sock = socker_.Accept(&clientip, &clienport, &err);
                if (sock >= 0)
                {
                    //读事件不能常设，读事件在大部分时间都是就绪的，在空间不足是再添加关心
                    //在sender中视情况添加
                    AddConnection(sock, EPOLLIN | EPOLLET,
                                  std::bind(&TcpServer::Recver, this, std::placeholders::_1),
                                  std::bind(&TcpServer::Sender, this, std::placeholders::_1),
                                  std::bind(&TcpServer::Excepter, this, std::placeholders::_1));
                }
                else
                {
                    if (err == EAGAIN || err == EWOULDBLOCK)
                        break;
                    else if (err == EINTR)
                        continue;
                    else
                        break;
                }
            }
        }

        void AddConnection(int sock, uint32_t events, func_t recver, func_t sender, func_t exceper)
        {
            // 设置为非阻塞
            // 如果需要
            if (events & EPOLLET)
                Util::SetNonBlock(sock);

            // 创建connection
            Connection *conn = new Connection(sock, this);
            // 注册对应的回调处理方法
            conn->Register(recver, sender, exceper);

            // 添加epoll模型
            bool r = epoller_.AddEvent(sock, events);
            assert(r);
            (void)r;

            // 管理起来
            connections_.insert(std::pair<int, Connection *>(sock, conn));
        }
        bool IsConnections(int sock)
        {
            return connections_.find(sock) != connections_.end();
        }
        void Loop(int timeout)
        {
            int n = epoller_.Wait(revs_, num_, timeout);
            if (n == 0)
            {
                logMessage(NORMAL, "timeout ...");
            }
            else if (n < 0)
            {
                logMessage(ERROR, "epoll_wait error, errno %d(%s)", errno, strerror(errno));
            }
            else
            {
                logMessage(NORMAL, "have event ready");
                // 将异常事件转换成读写事件，在读写事件中进行处理异常

                for (int i = 0; i < n; ++i)
                {
                    int sock = revs_[i].data.fd;
                    uint32_t events = revs_[i].events;

                    if (events & EPOLLERR)
                        events |= (EPOLLIN | EPOLLOUT);
                    if (events & EPOLLHUP)
                        events |= (EPOLLIN | EPOLLOUT);

                    if ((events & EPOLLIN) && IsConnections(sock) && connections_[sock]->recver_)
                        connections_[sock]->recver_(connections_[sock]);

                    if ((events & EPOLLOUT) && IsConnections(sock) && connections_[sock]->sender_)
                        connections_[sock]->sender_(connections_[sock]);
                }
            }
        }

    public:
        TcpServer(func_t service, uint16_t port = default_port) : service_(service), port_(port), revs_(nullptr), num_(0)
        {
        }
        void initServer()
        {
            // 创建socket
            socker_.Create_Socket();
            socker_.Bind(port_);
            socker_.Listen();

            // 创建epoll
            epoller_.Create();

            // 将目前唯一的一个sock，添加到epoller中， 之前需要先将对应的fd设置成为非阻塞
            // listensock_也是一个socket啊，也要看做成为一个Connection
            AddConnection(socker_.Get_Listensock(), EPOLLIN | EPOLLET,
                          std::bind(&TcpServer::Accepter, this, std::placeholders::_1), nullptr, nullptr);

            revs_ = new struct epoll_event[num];
            num_ = num;
        }

        void EnablieReadWrite(Connection *conn,bool readable,bool writeable)
        {
            uint32_t event= (readable?EPOLLIN:0)|(writeable?EPOLLOUT:0)|EPOLLET;
            epoller_.AddEvent(conn->sock_,event);
        }



        void Dispatch()
        {
            int timeout = -1;
            for (;;)
            {
                Loop(timeout);
            }
        }

        ~TcpServer()
        {
            if (revs_)
                delete[] revs_;
        }

    private:
        uint16_t port_;
        Socker socker_;
        Epoller epoller_;
        std::unordered_map<int, Connection *> connections_;
        struct epoll_event *revs_;
        int num_;
        func_t service_;
    };

}