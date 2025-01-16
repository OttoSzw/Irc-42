#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <stdlib.h>

#include <map>
#include "Utils.hpp"
#include "Client.hpp"

class Client;

class Channel
{
    private:

        std::string                 nameChannel;
        std::vector<Client *>     ChannelClient;

    public:

        Channel(std::string name);
        ~Channel();

        std::string                 getNameChannel() const;
        void                        addUser(Client *client);
        void                        removeUser(Client *client);

        const std::vector<Client *> &getClients() const;
};

#endif