#include <iostream>
#include <memory>
#include <string>
#include "tcpclient.hpp"
void Usage(char *proc)
{
    std::cout << "\nUsage:\n\t" << proc << " server_ip server_port\n\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(client::USAGE_ERR);
    }
    std::string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);
    std::unique_ptr<client::tcpclient> client(new client::tcpclient(serverip, serverport));
    client->initclient();
    client->start();
    return 0;
}