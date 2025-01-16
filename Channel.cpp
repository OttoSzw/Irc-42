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
    {
        ChannelClient.push_back(client);
        std::cout << "\033[0;35m[CHANNEL] \033[0m : \033[0;36m" << nameChannel << "\033[0m => " << client->GetNickname() << " has joined the channel !" << std::endl;
        std::cout << std::endl;
    }
}

void            Channel::removeUser(Client *client)
{
    if (std::find(ChannelClient.begin(), ChannelClient.end(), client) != ChannelClient.end())
    {
        ChannelClient.push_back(client);
        std::cout << "\033[0;35m[CHANNEL] \033[0m : \033[0;36m" << nameChannel << "\033[0m => " << client->GetNickname() << " has left the channel !" << std::endl;
        std::cout << std::endl;
    }
}

const std::vector<Client *> &Channel::getClients() const
{
    return (ChannelClient);
}
