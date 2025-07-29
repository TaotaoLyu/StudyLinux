#include <iostream>
#include <memory>
#include "pollServer.hpp"

static void Usage(char *proc)
{
    fprintf(stderr, "Usage:\n\t%s localport\n\n", proc);
}
std::string trancation(const std::string & request)
{
    return request;
}


int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    std::unique_ptr<poll_ns::pollServer> svr(new poll_ns::pollServer(trancation,atoi(argv[1])));

    svr->initServer();
    svr->start();

    return 0;
}