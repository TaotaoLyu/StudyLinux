#include <iostream>
#include <memory>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "httpServer.hpp"
#include "log.hpp"

void Usage(char *proc)
{
    std::cerr << "\nUsage:\n\t" << proc << " local_port\n\n";
}
std::string suffixToDesc(const std::string &suffix)
{
    std::string ret = "Content-Type: ";
    if (suffix == ".html")
    {
        ret += "text/html";
    }
    if (suffix == ".cs")
    {
        ret += "text/css";
    }
    if (suffix == ".js")
    {
        ret += "application/x-javascripe";
    }
    if (suffix == ".jpg")
    {
        ret += "image/jpeg";
    }
    return ret + "\r\n";
}
bool Get(const HttpRequest &req, HttpResponse &resp)
{
    std::cout << "-------------------HTTP BEGIN--------------------------\n";
    std::cout << req.inbuffer;
    std::cout << "method: " << req.method << std::endl;
    std::cout << "url: " << req.url << std::endl;
    std::cout << "httpversion: " << req.httpversion << std::endl;
    std::cout << "path: " << req.path << std::endl;
    std::cout << "size: " << req.file_size << std::endl;
    std::cout << "-------------------HTTP END----------------------------\n";

    // std::string respline = "HTTP/1.1 307 Temporary Redirect\r\n";
    std::string respline = "HTTP/1.1 200 OK\r\n";

    std::string respheader = suffixToDesc(req.suffix);
    respheader += "Content-Length: ";
    respheader += std::to_string(req.file_size);
    respheader += "\r\n";
    // respheader += "Location: https://www.gzhu.edu.cn/\r\n";
    respheader += "Set-Cookie: name=1234;max-age=120;path=/\r\n";

    std::string respblank = "\r\n";

    // resp.body_str = std::shared_ptr<char>(new char[req.file_size + 100]);
    std::string body;
    body.resize(req.file_size + 10);
    if (!Util::readFile(req.path, (char *)body.c_str(), body.size()))
    {
        logMessage(ERROR, "read error");
        return false;
    }
    resp.outbuffer += respline;
    resp.outbuffer += respheader;
    resp.outbuffer += respblank;

    resp.outbuffer += body;
    return true;
    // std::cout << "-------------------HTTP BEGIN--------------------------\n";
    // std::cout << resp.outbuffer;
    // std::cout<<"hello"<<std::endl;
    // std::cout << "-------------------HTTP END----------------------------\n";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(server::USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<server::httpServer> server(new server::httpServer(Get, port));

    server->initServer();

    server->start();

    return 0;
}