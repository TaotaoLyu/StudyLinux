#include <iostream>
#include <memory>
#include <unistd.h>
#include "tcpserver.hpp"
#include "daemon.hpp"
void Usage(char *pro)
{
    std::cout << "\nUsage:\n\t" << pro << " server_port\n\n";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(server::USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<server::tcpserver> server(new server::tcpserver(port));

    server->initserver();

    daemon();

    server->start();


    return 0;
}