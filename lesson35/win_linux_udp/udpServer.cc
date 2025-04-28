#include"udpServer.hpp"

#include<iostream>
#include<memory>
using namespace std;

void Usage(char *pro)
{

    cout<<"\nUsage:\n\t./udpServer server_port\n\n";
}


int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port=atoi(argv[1]);
    unique_ptr<udpServer> server(new udpServer(port));

    server->initServer();
    server->start();

    return 0;
}