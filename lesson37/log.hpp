#pragma once
#include <string>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include<cstdarg>

#define LOG_NORMAL "log.txt"
#define LOG_ERROR "log.error"
#define NUM 1024

enum Level
{
    DEBUG = 0,
    NORMAL,
    WARNING,
    ERROR,
    FATAL
};

static const char *to_level(Level level)
{
    switch (level)
    {
    case DEBUG:
        return "DEBUG";
    case NORMAL:
        return "NORMAL";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    case FATAL:
        return "FATAL";
    default:
        return NULL;
    }
}

void logMessage(Level level, const char *format,...)
{
    // std::cout << message << std::endl;
    //[NORMAL][2025-04-29 20:19:21][pid]message

    time_t now = time(0);
    struct tm *local_time = localtime(&now);

    char logprefix[NUM];
    char now_time[NUM];
    char logcontent[NUM];
    snprintf(now_time, sizeof(now_time),
             "%d-%02d-%02d %d:%d:%d",
             local_time->tm_year + 1900,
             local_time->tm_mon + 1,
             local_time->tm_mday,
             local_time->tm_hour,
             local_time->tm_min,
             local_time->tm_sec);

    snprintf(logprefix, sizeof(logprefix), "[%s][%s][pid: %d]", to_level(level), now_time, getpid());
    
    va_list args;
    va_start(args,format);
    vsnprintf(logcontent,sizeof(logcontent),format,args);

    std::cout<<logprefix<<logcontent<<std::endl;

    // FILE *lognormal=fopen(LOG_NORMAL,"a");
    // FILE *logerror=fopen(LOG_ERROR,"a");
    // if(logerror&&lognormal)
    // {
    //     FILE *current=nullptr;
    //     if(level==DEBUG||level==NORMAL||level==WARNING) current=lognormal;
    //     if(level==ERROR||level==FATAL) current=logerror;
        
        
    //     if(current)
    //         fprintf(current,"%s%s\n",logprefix,logcontent);
    //     fclose(lognormal);
    //     fclose(logerror);
    // }

}