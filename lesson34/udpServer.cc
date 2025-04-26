#include <stdio.h>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <signal.h>
#include "udpServer.hpp"
#include "onlineUsers.hpp"

using std::string;

void Usage(const std::string &proc)
{
    printf("\n\nUsage:\n\t%s local_port\n\n", proc.c_str());
}

const std::string dict_path = "./dict.txt";
std::unordered_map<std::string, std::string> dict;

static bool cutString(const string &target, string *key, string *value, const string &sep = ":")
{
    auto pos = target.find(sep);
    if (pos == string::npos)
        return false;
    *key = target.substr(0, pos);
    *value = target.substr(pos + sep.size());
    return true;
}

static void loaddict()
{
    std::ifstream in(dict_path, std::ios::binary);
    if (!in.is_open())
    {
        perror("open dict file");
        exit(Server::OPEN_ERR);
    }
    string line;
    string key, value;
    while (getline(in, line))
    {
        if (cutString(line, &key, &value))
            dict[key] = value;
    }
    in.close();
    std::cout << "load dict success\n";
}

void HotLoad(int signo)
{
    (void)signo;
    loaddict();
}

static void debugPrint()
{
    for (auto &e : dict)
    {
        std::cout << e.first << "->" << e.second << std::endl;
    }
}

void SendToOne(int sockfd, const std::string &clientip, const uint16_t &clientport, const string &message)
{

    struct sockaddr_in client;
    bzero(&client, sizeof(client));
    client.sin_port = htons(clientport);
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(clientip.c_str());
    sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&client, sizeof(client));
}

void handlerMessage(
    int sockfd,
    const std::string &clientip,
    const uint16_t &clientport,
    const std::string &message)
{
    string response="Reply Answer# ";
    if (dict.count(message))
        response += dict[message];
    else
        response = "unkown";

    SendToOne(sockfd, clientip, clientport, response);
}
void execCmd(int sockfd, const std::string &clientip, const uint16_t &clientport, const std::string &Cmd)
{
    string response;
    FILE *fp = popen(Cmd.c_str(), "r");
    if (fp == NULL)
    {
        response = strerror(errno);
        //response=Cmd+" exec failed";
    }
    else
    {
        char line[1024];
        while (fgets(line, sizeof(line), fp))
        {
            response += line;
        }
    }
    pclose(fp);
    response.resize(response.size()-1);
    SendToOne(sockfd, clientip, clientport, response);
}

online onlineusers(SendToOne);

void routeMessage(int sockfd, const std::string &clientip, const uint16_t &clientport, const std::string &message)
{
    if (message == "online")
    {
        onlineusers.AddUser(clientip, clientport);
    }
    if (message == "offline")
    {
        onlineusers.DelUser(clientip, clientport);
    }

    if (onlineusers.is_online(clientip, clientport))
    {
        onlineusers.Broadcast(sockfd, clientip, clientport, message);
    }
    else
    {
        SendToOne(sockfd, clientip, clientport, "您还未上线, 请运行 online");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(Server::USAGE_ERR);
    }
    // std::string ip=argv[1];

    // loaddict();
    // signal(SIGINT,HotLoad);
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<Server::udpServer> server(new Server::udpServer(handlerMessage,port));
    // std::unique_ptr<Server::udpServer> server(new Server::udpServer(execCmd,port));
    // std::unique_ptr<Server::udpServer> server(new Server::udpServer(routeMessage, port));

    server->initServer();
    server->start();

    return 0;
}