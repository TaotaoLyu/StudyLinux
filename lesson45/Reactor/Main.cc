
#include <memory>
#include <cstring>
#include <string>
#include "TcpServer.hpp"
#include "Protocol.hpp"
static void Usage(char *proc)
{
    fprintf(stderr, "Usage:\n\t%s localport\n\n", proc);
}

bool Calculator(const Protocol::Request &req, Protocol::Response &resp)
{
    // 处理业务逻辑
    resp.exitcode_ = Protocol::OK;
    resp.result_ = Protocol::OK;
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
            resp.exitcode_ = Protocol::DIV_ZERO;
        else
            resp.result_ = req.x_ / req.y_;
    }
    break;
    case '%':
    {
        if (req.y_ == 0)
            resp.exitcode_ = Protocol::MOD_ZERO;
        else
            resp.result_ = req.x_ % req.y_;
    }
    break;
    default:
        resp.exitcode_ = Protocol::OP_ERR;
        break;
    }
    return true;
}

void Service(tcpserver::Connection *conn)
{

    std::string recv_package;
    while (Protocol::ParseOneText(conn->inbuffer_, &recv_package))
    {
        // text是一个完整报文
        //  获取完整的数据,正确获取序列化的数据，不多读，不少读
        //  不多读，不少读
        std::string req_str;
        if (!Protocol::deLength(recv_package, &req_str))
            return;

        // 反序列，获得结构化的数据
        Protocol::Request req;
        if (!req.deserialize(req_str))
            return;

        // 业务处理，解耦
        Protocol::Response resp;
        Calculator(req, resp);

        // 序列化数据
        std::string resp_str;
        if (!resp.serialize(&resp_str))
            return;

        logMessage(DEBUG, "%d %c %d = %d(status: %d)", req.x_, req.op_, req.y_, resp.result_, resp.exitcode_);

        // 发送序列化数据
        conn->outbuffer_ += Protocol::enLength(resp_str);
    }

    if (conn->sender_)
        conn->sender_(conn);
    // if (conn->sender_)
    //     conn->sender_(conn);
    // if (!conn->outbuffer_.empty())
    //     conn->tsp_->EnablieReadWrite(conn, true, true);
    // else
    //     conn->tsp_->EnablieReadWrite(conn, true, false);
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    Util::daemon();
    uint16_t port = atoi(argv[1]);

    std::unique_ptr<tcpserver::TcpServer> tsvr(new tcpserver::TcpServer(Service, port));

    tsvr->initServer();

    tsvr->Dispatch();
    return 0;
}