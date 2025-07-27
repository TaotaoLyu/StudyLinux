#include<iostream>
#include<unistd.h>
#include<cstring>
#include<errno.h>
#include<functional>
#include<vector>

#include"util.hpp"

#define INIT(cbs) do{\
        cbs.push_back(printlog);\
        cbs.push_back(download);\
        cbs.push_back(executesql);\
    }while(0)


#define EXEC_OTHER(cbs) do{\
        for(const auto &cb:cbs)\
            cb();\
    }while(0)

using func_t=std::function<void()>;

int main()
{
    std::vector<func_t> cbs;
    INIT(cbs);
    char buffer[1024];
    setNonBlock(0);
    while(true)
    {
        printf(">>> ");
        fflush(stdout);
        ssize_t n=read(0,buffer,sizeof(buffer)-1);
        if(n>0)
        {
            buffer[n]=0;
            std::cout<<"echo# "<<buffer;
        }
        else if(n==0)
        {
            printf("read end!!!\n");
            break;
        }
        else //n==-1
        {
            
            // printf("errno->%d message->%s\n",errno,strerror(errno));
            //注意错误码的处理，不一定是真的错误
            if(errno==EAGAIN)
            {
                //数据没有准备好
                printf("no wrose, it is no ready!\n");
                EXEC_OTHER(cbs);
            }
            else if(errno==EINTR)
            {
                //读取被信号中断
                continue;
            }
            else
            {
                //真的错了
                printf("errno: %d(%s)\n",errno,strerror(errno));
                break;
            }

        }
        sleep(1);
    }

    return 0;
}


// 记得处理返回值, 返回值表示错误时, 要处理errno