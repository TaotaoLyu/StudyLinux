#include <iostream>
#include <memory>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include "httpServer.hpp"

void Usage(char *proc)
{
    std::cerr << "\nUsage:\n\t" << proc << " local_port\n\n";
}

bool Get(const HttpRequest &req, HttpResponse &resp)
{
    std::cout << "-------------------HTTP BEGIN--------------------------\n";
    std::cout << req.inbuffer;
    std::cout << "method: " << req.method << std::endl;
    std::cout << "url: " << req.url << std::endl;
    std::cout << "httpversion: " << req.httpversion << std::endl;
    std::cout << "path: " << req.path << std::endl;
    std::cout << "-------------------HTTP END----------------------------\n";

    std::string respline = "HTTP/1.1 200 OK\r\n";

    std::string respheader = "Content-Type: text/html\r\n";
    if(req.path.find(".css")!=std::string::npos)
        respheader = "Content-Type: text/css\r\n";
    if(req.path.find(".js")!=std::string::npos)
        respheader = "Content-Type: application/javascript\r\n";
    std::string respblank = "\r\n";
    
    std::string body;
    FILE *fp = fopen(req.path.c_str(), "r");
    if(!fp) return false;
    char buffer[4096];
    while (true)
    {
        char *ret = fgets(buffer, sizeof(buffer), fp);
        if (!ret)
            break;
        body += buffer;
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