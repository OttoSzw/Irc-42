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

class Client
{
    private:

        int             clientFd;
        std::string     nickname;
        std::string     username;

        bool            setNick;
        bool            setUser;

    public:

        Client(int client_fd);

        std::string            GetNickname();
        std::string            GetUsername();
        void            SetNickname(std::string newname);
        void            SetUsername(std::string firstname, std::string lastname);

        std::string     recvMessage();
        int            Authentication(std::string password, int sizeTab, std::string element);
};

#endif