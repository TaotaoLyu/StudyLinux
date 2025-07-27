#pragma once


#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include<cerrno>
#include<cstring>

void setNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        fprintf(stderr,"fcntl error %d(%s)\n",errno,strerror(errno));
        return;
    }

    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

void printlog()
{
    printf("this is log!!!\n");
}

void download()
{
    printf("download file...\n");
}

void executesql()
{
    printf("executesql ...\n");
}