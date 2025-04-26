#pragma once
#include<string>
#include<functional>
#include<unordered_map>

using std::string;

struct User
{
    string _userip;
    uint16_t _userport;
    User()=default;
    User(const string &userip,const uint16_t &userport)
    :_userip(userip),_userport(userport)
    {}
    ~User()=default;
};
class online
{
    using func_t=std::function<void(int,const std::string &,const uint16_t &,const string &)>;
public:
    online(func_t SendToOne)
    :_SendToOne(SendToOne)
    {}
    ~online()=default;
    void AddUser(const string &userip,const uint16_t &userport)
    {
        string key=userip+":"+std::to_string(userport);
        _users[key]=User(userip,userport);
    }
    void DelUser(const string &userip,const uint16_t &userport)
    {
        string key=userip+":"+std::to_string(userport);
        _users.erase(key);
    }
    bool is_online(const string &userip,const uint16_t &userport)
    {
        string key=userip+":"+std::to_string(userport);
        return _users.count(key);
    }
    void Broadcast(int sockfd,const string &userip,const uint16_t &userport,const string& message)
    {
        string key=userip+":"+std::to_string(userport);
        for(auto &user:_users)
        {
            string response=key+"# "+message;
            _SendToOne(sockfd,user.second._userip,user.second._userport,response);
        }

    }
private:
    std::unordered_map<string,User> _users;
    func_t _SendToOne;
};