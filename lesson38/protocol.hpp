#pragma once
#include <string>
#include <sstream>
#include "Util.hpp"

const std::string sep = "\r\n";
const std::string default_root = "./wwwroot";
const std::string home_page = "index.html";

class HttpRequest
{
public:
    HttpRequest() = default;
    ~HttpRequest() = default;
    void parase()
    {
        std::string line = Util::getOneLine(inbuffer, sep);

        if (line.empty())
            return;
        std::cout << "line: " << line << std::endl;
        std::stringstream ss(line);
        ss >> method >> url >> httpversion;

        path = default_root;
        path += url;
        if (path.back() == '/')
            path += home_page;
    }

public:
    std::string inbuffer;

    std::string method;
    std::string url;
    std::string httpversion;
    std::string path;
};

class HttpResponse
{

public:
    std::string outbuffer;
};
