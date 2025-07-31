
#include<memory>
#include<cstring>
#include"TcpServer.hpp"
static void Usage(char *proc)
{
    fprintf(stderr, "Usage:\n\t%s localport\n\n", proc);
}
int main(int argc,char *argv[])
{


    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port=atoi(argv[1]);

    std::unique_ptr<tcpserver::TcpServer> tsvr(new tcpserver::TcpServer(port));

    tsvr->initServer();

    tsvr->Dispatch();
    return 0;
}