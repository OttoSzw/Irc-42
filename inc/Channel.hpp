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

        int                             _operator;
        std::string                     nameChannel;
        std::vector<Client *>           ChannelClient;
        std::string                     topic;

    public:

                                        Channel(std::string name);
                                        ~Channel();

        std::string                     getNameChannel(void) const;
        const std::vector<Client *>     &getClients() const;
        int                             getOperator(void) const;
        std::string                     getTopic() const;

        void                            setOperator(int newInt, std::string name);
        void                            setNameChannel(std::string newString);
        void                            setTopic(std::string newTopic);

        void                            addUser(Client *client);
        void                            removeUser(Client *client);
        int                             isOperator(Client *client);
        void                            Broadcast(std::string message);

        int                             isUserInChannel(Client *clientToInvite);
};

#endif