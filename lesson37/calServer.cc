#include <iostream>
#include <memory>
#include <unistd.h>
#include "calServer.hpp"

void Usage(char *pro)
{
    std::cout << "\nUsage:\n\t" << pro << " local_port\n\n";
}

bool cal(const Request &req, Response &resp)
{
    // 处理业务逻辑
    resp.exitcode_ = OK;
    resp.result_ = OK;
    switch (req.op_)
    {
    case '+':
        resp.result_ = req.x_ + req.y_;
        break;
    case '-':
        resp.result_ = req.x_ - req.y_;
        break;
    case '*':
        resp.result_ = req.x_ * req.y_;
        break;
    case '/':
    {
        if (req.y_ == 0)
            resp.exitcode_ = DIV_ZERO;
        else
            resp.result_ = req.x_ / req.y_;
    }
    break;
    case '%':
    {
        if (req.y_ == 0)
            resp.exitcode_ = MOD_ZERO;
        else
            resp.result_ = req.x_ % req.y_;
    }
    break;
    default:
        resp.exitcode_ = OP_ERR;
        break;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(server::USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<server::calServer> server(new server::calServer(port));

    server->initServer();

    server->start(cal);

    return 0;
}