
#include<memory>

#include"udpClient.hpp"

void Usage(const std::string &proc)
{
    printf("\n\nUsage:\n\t%s server_ip server_port\n\n",proc.c_str());
}

int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip=argv[1];
    uint16_t serverport=atoi(argv[2]);
    std::unique_ptr<Client::udpClient> client(new Client::udpClient(serverip,serverport));
    client->initClient();
    client->run();



    return 0;
}