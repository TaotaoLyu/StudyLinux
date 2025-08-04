#pragma once

#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "Log.hpp"

class Util
{
    #define DEV "/dev/null"
public:
    static bool SetNonBlock(int sock)
    {
        int fl = fcntl(sock, F_GETFL);
        if (fl < 0)
        {
            logMessage(FATAL, "fcntl error, errno %d(%s)", errno, strerror(errno));
            return false;
        }
        fcntl(sock, F_SETFL, fl | O_NONBLOCK);
        return true;
    }
    static void daemon(const char *currpath = nullptr)
    {

        //send出异常不退出
        signal(SIGPIPE, SIG_IGN);

        // 让自己不是组长
        if (fork() > 0)
            exit(0);

        // 创建新的会话
        pid_t n = setsid();
        assert(n != -1);

        // 文件描述符重定向
        int fd = open(DEV, O_RDWR);
        if (fd >= 0)
        {
            dup2(fd, 0);
            dup2(fd, 1);
            dup2(fd, 2);

            close(fd); //!!!!!
        }
        else
        {
            close(0);
            close(1);
            close(2);
        }
        if (currpath)
            chdir(currpath);
        
        logMessage(NORMAL,"create daemon process success...");
    }
};