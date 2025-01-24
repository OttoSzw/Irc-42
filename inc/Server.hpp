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
#include "signal.h"


class Server
{

    private:
    
        void botTask();
        int                         port;
        std::string                 password;

        bool                        valid;
        int                         socketServer;
        int                         epollFd;

        std::map<int, Client *>     ClientsList;
        std::vector<Channel *>      ChannelList;

    public:

        Server(int PortGiven, std::string PasswordGiven);

        std::map<int, Client*>& getClientsList()
        {
                return (ClientsList);
        }
        std::vector<Channel *>& getChannelList()
        {
                return (ChannelList);
        }
        void    RunningServer();
        void    newConnection();
        void    handleConnection(int client_fd);
        void sendPeriodicMessage();
        void sendMessageToServer(const std::string& message);
};

#endif