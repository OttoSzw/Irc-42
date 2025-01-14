#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <netinet/in.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

class Server
{

    private:

        int                         port;
        std::string                 password;

        bool                        valid;
        int                         socketServer;
        int                         epollFd;

        std::map<int, Client *>     ClientsList;
        std::vector<Channel *>      ChannelList;

    public:

        Server(int PortGiven, std::string PasswordGiven);

        void    RunningServer();
        void    newConnection();
        void    handleConnection(int client_fd);
};

#endif