#include<iostream>
#include<unistd.h>
#include<cassert>
#include<cstring>
#include<sys/types.h>
#include<sys/wait.h>


int main()
{

    int fds[2];
    int ret=pipe(fds);
    assert(ret==0);
    pid_t id=fork();
    if(id==0)
    {
        //child process
        close(fds[0]);
        //sent message
        int cnt=0;
        while(true)
        {
            char buffer[1024];
            snprintf(buffer,sizeof buffer -1,
                "I am child process, my pid is %d, cnt: %d\n",getpid(),cnt++);
            write(fds[1],buffer,strlen(buffer));
            if(cnt==60) break;
            //std::cout<<"cnt: "<<cnt<<std::endl;
            // sleep(1);
        }
        //
        close(fds[1]);
        return 0;
    }
    //father massage
    close(fds[1]);
    //receive message
    while(true)
    {
        //sleep(20000);
        sleep(5);
        char buffer[1024];
        std::cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
        int num=read(fds[0],buffer,sizeof buffer -1);
        std::cout<<"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n";
        if(num==0) break;
        buffer[num]='\0';
        printf("receive meaasge: %s\n",buffer);
        //break;
        
    }
    close(fds[0]);



    int status=0;
    //reclaim child procsee
    ret=waitpid(id,&status,0);
    assert(ret==id);
    std::cout<<"status: "<<(status&0x7F)<<std::endl;
    return 0;
}


//if write slowly and read quickly , read will be blocked(defalut)
//if write quickly and read slowly, buffer will full of data waiting to be read
//write will be blocked

//if read side is closed early,the process on the write side will exit exceptional
//if write side is closed early, the function of read will return zero