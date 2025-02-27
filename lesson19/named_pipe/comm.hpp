#pragma once

#include<iostream>
#include<cstdio>
#include<cstring>
#include<errno.h>
#include<unistd.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define NAMED_PIPE "/tmp/named.pipe"

bool createFifo(const char* path)
{
    umask(0);
    int ret=mkfifo(path,0600);
    if(ret==0) return true;
    else
    {
        std::cout<<"error: "<<strerror(errno)<<std::endl;
        return false;
    }
}

void removeFifo(const char* path)
{
    int ret=unlink(path);
    assert(ret==0);
    (void)ret;
}