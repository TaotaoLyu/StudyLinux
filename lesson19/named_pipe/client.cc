#include"comm.hpp"

int main()
{
    std::cout<<"client begin\n";
    int wfd=open(NAMED_PIPE,O_WRONLY);
    std::cout<<"client end\n";

    assert(wfd>=0);

    while(true)
    {
        std::cout<<"Please Say #";
        char buffer[1024];
        fgets(buffer,sizeof(buffer),stdin);
        if(strlen(buffer)>0) buffer[strlen(buffer)-1]='\0';
        ssize_t n=write(wfd,buffer,strlen(buffer));
        assert(n==strlen(buffer));
        (void)n;
    }
    close(wfd);
    return 0;
}