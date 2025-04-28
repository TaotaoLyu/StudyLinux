#pragma once
#include <string>
#include <iostream>

enum Level
{
    DEBUG = 0,
    NORMAL,
    WARNING,
    ERROR,
    FATAL
};

void logMessage(Level level, const std::string &message)
{
    std::cout << message << std::endl;
}