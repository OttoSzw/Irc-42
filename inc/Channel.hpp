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
        bool                            _inviteOnly;
        bool                            _topicRestrict;
        int                             _userLimit;
        std::string                     _key;
        std::string                     nameChannel;
        std::vector<Client *>           ChannelClient;
        std::vector<Client *>           _listInvite;
        std::string                     topic;

    public:

                                        Channel(std::string name);
                                        ~Channel();

        std::string                     getNameChannel(void) const;
        const std::vector<Client *>     &getClients() const;
        int                             getOperator(void) const;
        std::string                     getTopic() const;
        bool                            getInviteOnly() const;
        bool                            gettopicRestrict() const;
        int                             getUserLimit(void) const;
        std::string                     getKey(void) const;

        void                            setOperator(int newInt, std::string name);
        void                            setNameChannel(std::string newString);
        void                            setTopic(std::string newTopic);
        void                            setInvitOnly(bool newBool);
        void                            settopicRestrict(bool newBool);
        void                            setUserLimit(int newInt);
        void                            setKey(std::string newString);

        void                            addUser(Client *client);
        void                            addUserInvite(Client *client);
        void                            removeUser(Client *client);
        void                            removeUserInvite(Client *client);
        int                            InviteList(Client *client);
        int                             isOperator(Client *client);
        void                            Broadcast(std::string message);
        int                             CheckKey(std::string newString);

        int                             isUserInChannel(Client *clientToInvite);
        int                             findUser(std::string name);
};

#endif