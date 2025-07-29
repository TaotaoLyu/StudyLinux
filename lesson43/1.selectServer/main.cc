#include <iostream>
#include <memory>
#include "selectServer.hpp"

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

    std::cout<<sizeof(fd_set)<<std::endl;
    std::unique_ptr<select_ns::selectServer> svr(new select_ns::selectServer(trancation,atoi(argv[1])));

    svr->initServer();
    svr->start();

    return 0;
}