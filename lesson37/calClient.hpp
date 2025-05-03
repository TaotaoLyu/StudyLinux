#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "protocol.hpp"

namespace client
{
    using namespace std;

    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        CONNECT_ERR,
    };
    int gnum = 1024;
    class calClient
    {
    public:
        calClient(const string &serverip, const uint16_t &serverport)
            : _serverip(serverip), _serverport(serverport), _sock(-1)
        {
        }
        void initClient()
        {
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_sock < 0)
            {
                cerr << "socket create error" << endl;
                exit(SOCKET_ERR);
            }
        }
        void start()
        {
            struct sockaddr_in server;
            bzero(&server, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(_serverip.c_str());
            server.sin_port = htons(_serverport);
            // 连接失败
            if (connect(_sock, (struct sockaddr *)&server, sizeof(server)) != 0)
            {
                cerr << "socket connect error" << endl;
            }
            else
            {
                // 这里都用continue
                //  错了重新输入
                string line;
                std::string inbuffer;
                while (true)
                {
                    cout << "MyCal >>> ";
                    getline(cin, line);

                    Request req = ParaseLine(line);

                    std::string req_str;
                    if (!req.serialize(&req_str))
                        continue;

                    std::string send_package = enLength(req_str);
                    send(_sock, send_package.c_str(), send_package.size(), 0);
                    // send(_sock, send_package.c_str(), send_package.size()-3, 0);

                    std::string recv_package, resp_str;
                    if (!recvPackage(_sock, inbuffer, &recv_package))
                        continue;

                    if (!deLength(recv_package, &resp_str))
                        continue;

                    Response resp;
                    if (!resp.deserialize(resp_str))
                        continue;

                    cout << "exitcode: " << resp.exitcode_ << endl;
                    cout << "result: " << resp.result_ << endl;
                }
            }
        }
        // " x op y"
        Request ParaseLine(const string &line)
        {
            int status = 0; // 0表示op前面 1表示op 2表示op后面
            int i = 0;
            int cnt = line.size();
            char op;
            string left, right;
            while (i < cnt)
            {
                switch (status)
                {
                case 0:
                {
                    if (isdigit(line[i]))
                    {
                        left.push_back(line[i++]);
                    }
                    else
                    {
                        status = 1;
                        op = line[i];
                    }
                }
                break;
                case 1:
                    i++;
                    status = 2;
                    break;
                case 2:
                    right.push_back(line[i++]);
                    break;
                }
            }

            return Request(std::stoi(left), op, std::stoi(right));
        }
        ~calClient()
        {
            if (_sock >= 0)
                close(_sock);
        }

    private:
        string _serverip;
        uint16_t _serverport;
        int _sock;
    };

}
