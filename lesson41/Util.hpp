#pragma once
#include <string>
#include<fstream>

class Util
{
public:
    static std::string getOneLine(std::string &buffer, const std::string &sep)
    {
        auto pos = buffer.find(sep);
        if (pos == std::string::npos)
            return "";
        std::string sub = buffer.substr(0, pos);
        // buffer.erase(0,sub.size()+sep.size());// 注意
        return sub;
    }
    static bool readFile(const std::string &path, char *buffer,int n)
    {
        std::ifstream in(path,std::ios::binary);
        if(!in.is_open()) return false;
        in.read(buffer,n);

        in.close();
        // int fd=open(path.c_str(),O_RDONLY);
        // if(fd<0) return false;
        // int n = 0;
        // while (true)
        // {
        //     int ret = read(fd, buffer.get() + n, sizeof(buffer));
        //     n += ret;
        //     if (ret == 0)
        //         break;
        // }
        return true;
    }
};