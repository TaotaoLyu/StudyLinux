#pragma once

#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include "Log.hpp"

class Util
{
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
};