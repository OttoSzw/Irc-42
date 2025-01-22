#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>

#include <sys/socket.h>
#include <sys/epoll.h>

#include <map>
#include <netinet/in.h>
#include "Channel.hpp"
#include "Utils.hpp"

class Channel;

class Client
{
    private:

        int             clientFd;
        std::string     nickname;
        std::string     username;

        bool            setNick;
        bool            setUser;


    public:

        bool                        flagWelcome;
        Client(int client_fd);

        std::string            GetNickname();
        std::string            GetUsername();
        int                    GetClientFd();
        void            SetNickname(std::string newname);
        void            SetUsername(std::string firstname, std::string lastname);

        std::string     recvMessage();
        int             Authentication(std::string password, int sizeTab, std::string element);
        void            PrivMsg(const std::map<int, Client *> &ClientsList, const std::vector<Channel *> &ChannelList, std::string target, std::string message);
        void            Ping(std::string token);
        void            SetMode(std::vector<std::vector<std::string> > av, int i, std::vector<Channel *> ChannelList);
        void            JoinChannel(std::string nameChannel, std::vector<Channel *> &ChannelList, std::string password);
        void            Invite(std::string nameUser, std::string nameChannel, std::vector<Channel *> ChannelList, const std::map<int, Client *> &ClientsList);
        void            SetTopic(std::string channel, std::vector<Channel *> ChannelList, std::string newTopic);
        void            Kick(std::string channel, std::string user, const std::map<int, Client *> &ClientsList, std::vector<Channel *> ChannelList, std::string message);
};

#endif