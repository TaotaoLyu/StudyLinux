#pragma once
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>


class udpServer
{
public:
    udpServer(const uint16_t &serverport)
    :_serverport(serverport),_sockfd(-1)
    {}
    void initServer()
    {
        _sockfd=socket(AF_INET,SOCK_DGRAM,0);
        if(_sockfd==-1)
        {
            perror("socket ");
            exit(1);
        }
        
        struct sockaddr_in local;
        bzero(&local,sizeof(local));
        local.sin_family=AF_INET;
        local.sin_addr.s_addr=INADDR_ANY;
        local.sin_port=htons(_serverport);
        int n=bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
        if(n==-1)
        {
            perror("socket ");
            exit(1);
        }
    }
    void start()
    {
        struct sockaddr_in client;
        bzero(&client,sizeof(client));
        socklen_t len=sizeof(client);
        while (true)
        {
            char buffer[1024];
            ssize_t n=recvfrom(_sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&client,&len);
            std::string clientip=inet_ntoa(client.sin_addr);
            uint16_t clientport=ntohs(client.sin_port);
            if(n>=0)
            {
                buffer[n]=0;
                std::cout<<"["<<clientip<<":"<<clientport<<"]"<<"# "<<buffer<<std::endl;
                sendto(_sockfd,buffer,n,0,(struct sockaddr*)&client,len);
            }
            else
                break;
        }
    }
    ~udpServer()
    {
        if(_sockfd>=0)
            close(_sockfd);
    }
private:
    uint16_t _serverport;
    int _sockfd;
};