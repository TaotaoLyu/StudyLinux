#pragma once
#include <string>
#include <memory>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Util.hpp"

const std::string sep = "\r\n";
const std::string default_root = "./wwwroot";
const std::string home_page = "index.html";
const std::string html_404 = "./wwwroot/404.html";

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

        auto pos = path.rfind(".");
        if (pos == std::string::npos)
            suffix = ".html";
        else
            suffix = path.substr(pos);

        struct stat file;
        if (stat(path.c_str(), &file) == 0)
        {
            file_size = file.st_size;
        }
        else
        {
            path = html_404;
            stat(path.c_str(), &file);
            file_size = file.st_size;
            suffix = ".html";
        }
    }

public:
    std::string inbuffer;

    std::string method;
    std::string url;
    std::string httpversion;
    std::string path;
    std::string suffix;
    int file_size;
};

class HttpResponse
{
public:
    // void serialize()
    // {
    //     package_len = (int)outbuffer.size() + body_len;
    //     package = new char[package_len + 10];

    //     memcpy(package, outbuffer.c_str(), outbuffer.size());
    //     memcpy(package + outbuffer.size(), body_str.get(), body_len);
    //     body_str.~shared_ptr();
    // }

public:
    std::string outbuffer;
    // std::shared_ptr<char> body_str;
    // int body_len;
    // char *package = NULL;
    // int package_len;
};
