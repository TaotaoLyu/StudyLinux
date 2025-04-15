#include<stdio.h>
#include<memory>
#include"udpServer.hpp"


void Usage(const std::string &proc)
{
    printf("\n\nUsage:\n\t%s local_port\n\n",proc.c_str());
}

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(Server::USAGE_ERR);
    }
    // std::string ip=argv[1];
    uint16_t port=atoi(argv[1]);
    std::unique_ptr<Server::udpServer> server(new Server::udpServer(port));

    server->initServer();
    server->start();

    return 0;
}