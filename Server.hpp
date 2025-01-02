#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>

#include <sys/socket.h>
#include <sys/epoll.h>

#include <map>
#include <netinet/in.h>
#include "Client.hpp"

class Server
{

    private:

        int port;
        std::string password;
        int                     socketServer;
        std::map<int, Client *> ClientsList;

    public:

        Server(int PortGiven, std::string PasswordGiven);

        void    RunningServer();
        void    newConnection(int epollFd);
        void    handleConnection(int epollFd, epoll_event *events, int i);
};

#endif