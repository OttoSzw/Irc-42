#include "Channel.hpp"

Channel::Channel(std::string name) : nameChannel(name)
{

}

Channel::~Channel()
{

}

std::string     Channel::getNameChannel() const
{
    return (nameChannel);
}

void            Channel::addUser(Client *client)
{
    if (std::find(ChannelClient.begin(), ChannelClient.end(), client) == ChannelClient.end())
        ChannelClient.push_back(client);
}

void            Channel::removeUser(Client *client)
{
    if (std::find(ChannelClient.begin(), ChannelClient.end(), client) != ChannelClient.end())
        ChannelClient.push_back(client);
}