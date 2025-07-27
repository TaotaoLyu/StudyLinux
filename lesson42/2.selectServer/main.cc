#include <iostream>
#include <memory>
#include "selectServer.hpp"

static void Usage(char *proc)
{
    fprintf(stderr, "Usage:\n\t%s localport\n\n", proc);
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    std::unique_ptr<select_ns::selectServer> svr(new select_ns::selectServer(atoi(argv[1])));

    svr->initServer();
    svr->start();

    return 0;
}