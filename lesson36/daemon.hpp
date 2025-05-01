#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<signal.h>
#include <fcntl.h>
#include <stdlib.h>

#define DEV "/dev/null"

void daemon(const char *currpath = nullptr)
{

    //??????
    signal(SIGPIPE,SIG_IGN);


    // 让自己不是组长
    if (fork() > 0)
        exit(0);

    // 创建新的会话
    pid_t n=setsid();
    assert(n!=-1);


    // 文件描述符重定向
    int fd = open(DEV, O_RDWR);
    if (fd >= 0)
    {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);

        close(fd);//!!!!!
    }
    else
    {
        close(0);
        close(1);
        close(2);
    }
    if(currpath) chdir(currpath);
}