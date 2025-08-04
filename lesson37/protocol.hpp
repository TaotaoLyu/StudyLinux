#pragma once

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <jsoncpp/json/json.h>

enum STATUS
{
    OK = 0,
    DIV_ZERO,
    MOD_ZERO,
    OP_ERR
};

#define SEP " "
#define SEP_LEN strlen(SEP)
#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

// "content" -> "content_len"\r\n"content"\r\n
// "1 + 2"  -> 5\r\n1 + 2\r\n
std::string enLength(const std::string &content)
{
    std::string send_package = std::to_string(content.size());
    send_package += LINE_SEP;
    send_package += content;
    send_package += LINE_SEP;

    return send_package;
}
//  "content_len"\r\n"content"\r\n -> "content"
bool deLength(const std::string &package, std::string *content)
{
    auto pos = package.find(LINE_SEP);
    if (pos == std::string::npos)
        return false;

    int content_len = std::stoi(package.substr(0, pos));

    *content = package.substr(pos + LINE_SEP_LEN, content_len);

    return true;
}

class Request
{
public:
    Request() = default;
    Request(int x, char op, int y)
        : x_(x), y_(y), op_(op)
    {
    }
    // 可能输入的数据有问题，导致return false
    // "x op y"
    bool serialize(std::string *out)
    {
#ifdef MYSELF
        if (!out)
            return false;

        *out = "";
        std::string x_string = std::to_string(x_);
        std::string y_string = std::to_string(y_);

        *out = x_string;
        *out += SEP;
        *out += op_;
        *out += SEP;
        *out += y_string;
#else
        Json::Value root;
        root["x"] = x_;
        root["y"] = y_;
        root["op"] = op_;

        // Json::FastWriter write;
        Json::StyledWriter write;
        *out = write.write(root);
#endif

        return true;
    }
    // "x  op  y"->struct data
    bool deserialize(const std::string &in)
    {
#ifdef MYSELF
        if (in.empty())
            return false;
        auto left = in.find(SEP);
        auto right = in.rfind(SEP);

        // 保证健壮性
        if (left == std::string::npos || right == std::string::npos)
            return false;
        if (left == right)
            return false;
        if (right - (left + SEP_LEN) != 1)
            return false;

        std::string x_string = in.substr(0, left);
        std::string y_string = in.substr(right + SEP_LEN);

        if (x_string.empty() || y_string.empty())
            return false;
        x_ = std::stoi(x_string);
        y_ = std::stoi(y_string);
        op_ = in[left + SEP_LEN];
#else
        Json::Value root;
        Json::Reader read;
        read.parse(in, root);
        x_ = root["x"].asInt();
        y_ = root["y"].asInt();
        op_ = root["op"].asInt();
#endif
        return true;
    }
    ~Request() = default;

public:
    int x_;
    int y_;
    char op_;
};

class Response
{
public:
    Response() = default;
    Response(STATUS exitcode, int result)
        : exitcode_(exitcode), result_(result)
    {
    }
    // 可能输入的数据有问题，导致return false
    // "exitcode result"
    bool serialize(std::string *out)
    {
#ifdef MYSELF
        if (!out)
            return false;
        *out = "";
        std::string exitcode_string = std::to_string(exitcode_);
        std::string result_string = std::to_string(result_);

        *out = exitcode_string;
        *out += SEP;
        *out += result_string;
#else
        Json::Value root;
        root["exitcode"] = exitcode_;
        root["result"] = result_;
        // Json::FastWriter write;
        Json::StyledWriter write;
        *out = write.write(root);
#endif
        return true;
    }
    // "exitcode result" ->
    bool deserialize(const std::string &in)
    {
#ifdef MYSELF
        if (in.empty())
            return false;
        auto mid = in.find(SEP);
        if (mid == std::string::npos)
            return false;

        std::string exitcode_string = in.substr(0, mid);
        std::string result_string = in.substr(mid + SEP_LEN);

        if (exitcode_string.empty() || result_string.empty())
            return false;

        exitcode_ = (STATUS)std::stoi(exitcode_string);
        result_ = std::stoi(result_string);
#else
        Json::Value root;
        Json::Reader read;
        read.parse(in, root);
        exitcode_ = (STATUS)root["exitcode"].asInt();
        result_ = root["result"].asInt();
#endif
        return true;
    }
    ~Response() = default;

public:
    STATUS exitcode_;
    int result_;
};

// "content_len"\r\n"content"\r\n"content_len"\r\n"content"\r\n"content_len"\r\n"content"\r\n
// "content
//  "content_len"\r\n"con
// -> "content_len"\r\n"content"\r\n
#include <iostream>
bool recvPackage(int sock, std::string &inbuffer, std::string *package)
{
    char buffer[1024];
    while (true)
    {
        ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            inbuffer += buffer;

            auto pos = inbuffer.find(LINE_SEP);
           // std::cout << "inbufer:\n"
           //           << inbuffer << std::endl;
            if (pos == std::string::npos)
                continue;
                
            // 一定有content_len
            std::string len_string = inbuffer.substr(0, pos);
            int content_len = std::stoi(len_string);
            int total_len = len_string.size() + 2 * LINE_SEP_LEN + content_len;
            if (total_len > inbuffer.size())
                continue;

            // 一定有一个完整的package
            *package = inbuffer.substr(0, total_len);
            inbuffer.erase(0, total_len);
            break;
        }
        else
            return false;
    }
    return true;
}
