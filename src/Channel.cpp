#include "Channel.hpp"

Channel::Channel(std::string name) : nameChannel(name)
{

}

Channel::~Channel()
{

}

const std::vector<Client *> &Channel::getClients() const
{
    return (ChannelClient);
}

std::string     Channel::getNameChannel() const{
    return (nameChannel);
}

int     Channel::getOperator(void) const
{
    return(this->_operator);
}

std::string     Channel::getTopic() const
{
    return (topic);
}

void    Channel::setOperator(int newInt, std::string name)
{
    std::cout << "\033[46m[Operator] " << name << " has become an operator of " << nameChannel << "\033[0m" << std::endl;
    this->_operator = newInt;
    topic = "Nothing";
}

void    Channel::setNameChannel(std::string newString)
{
    this->nameChannel = newString;
}

void    Channel::setTopic(std::string newTopic)
{
    this->topic = newTopic;
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
        std::cout << "\033[0;37m[CHANNEL] \033[0m : \033[0;36m" << nameChannel << "\033[0m => " << client->GetNickname() << " has left the channel !" << std::endl;
        std::cout << std::endl;
    }
}

int             Channel::isOperator(Client *client)
{
    if (client->GetClientFd() == _operator)
        return (1);
    return (0);
}

void            Channel::Broadcast(std::string message)
{
    for (Client *client : ChannelClient)
    {
        if (client != NULL)
            sendMessage(client->GetClientFd() , message);   
    }
}