#include"comm.hpp"

int main()
{
    bool ret=createFifo(NAMED_PIPE);
    assert(ret);
    (void)ret;
    std::cout<<"server begin\n";
    int rfd=open(NAMED_PIPE,O_RDONLY);
    std::cout<<"server end\n";
    assert(rfd>=0);
    // if(rfd<0) exit(1);

    while(true)
    {
        char buffer[1024];
        
        ssize_t n=read(rfd,buffer,sizeof buffer - 1);
        std::cout<<"server end\n";
        if(n>0) 
        {
            //std::cout<<strlen(buffer)-1<<std::endl;
            buffer[n]='\0';
            std::cout<<"client -> server # "<<buffer<<std::endl;
            
        }
        else if(n==0)
        {
            std::cout<<"client quit! Me too!!\n";
            break;

        }
        else
        {
            std::cout<<"error string : "<<strerror(errno)<<std::endl;
            break;
        }


    }




    close(rfd);
    removeFifo(NAMED_PIPE);

    return 0;
}