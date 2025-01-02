#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>

#include <sys/socket.h>
#include <sys/epoll.h>

#include <map>
#include <netinet/in.h>

class Client
{
    private:


    public:

        std::string recvMessage();
};

#endif