
#include<iostream>
#include<cstring>
#include<memory>
#include"epollServer.hpp"
#include"err.hpp"

static void Usage(char *proc)
{
    fprintf(stderr, "Usage:\n\t%s localport\n\n", proc);
}

// ./epollServer 8000
int main(int argc,char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    uint16_t port=atoi(argv[1]);
    std::unique_ptr<epoll_ns::epollServer> epollsvr(new epoll_ns::epollServer(port));

    epollsvr->initServer();

    epollsvr->start();


    return 0;
}