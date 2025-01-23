#include "Channel.hpp"

Channel::Channel(std::string name) : nameChannel(name)
{
    this->_inviteOnly = false;
    this->_userLimit = -1;
    this->_topicRestrict = false;
    this->_key = "Unknow";
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

bool    Channel::getInviteOnly() const{
    return(this->_inviteOnly);
}

int Channel::getUserLimit(void) const{
    return (this->_userLimit);
}

std::string Channel::getKey(void) const{
    return(this->_key);
}

bool    Channel::gettopicRestrict(void) const{
    return(this->_topicRestrict);
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

void    Channel::setInvitOnly(bool newBool){
    this->_inviteOnly = newBool;
}

void    Channel::setUserLimit(int newInt){
    this->_userLimit = newInt;
}

void    Channel::setKey(std::string newString){
    this->_key = newString;
}

void    Channel::settopicRestrict(bool newBool){
    this->_topicRestrict = newBool;
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

void            Channel::addUserInvite(Client *client)
{
    if (std::find(_listInvite.begin(), _listInvite.end(), client) == _listInvite.end())
    {
        _listInvite.push_back(client);
        std::cout << "\033[0;35m[CHANNEL] \033[0m : \033[0;36m" << nameChannel << "\033[0m => " << client->GetNickname() << " has been invite to the channel !" << std::endl;
        std::cout << std::endl;
    }
}

int            Channel::InviteList(Client *client)
{
    if (std::find(_listInvite.begin(), _listInvite.end(), client) == _listInvite.end())
        return (0);
    return (1);
}

void            Channel::removeUser(Client *client)
{
    std::vector<Client*>::iterator it = std::find(ChannelClient.begin(), ChannelClient.end(), client);
    if (it != ChannelClient.end())
    {
        ChannelClient.erase(it);
        std::cout << "\033[0;37m[CHANNEL] \033[0m : \033[0;36m" << nameChannel << "\033[0m => " << client->GetNickname() << " has left the channel !" << std::endl;
        std::cout << std::endl;
    }
}

void            Channel::removeUserInvite(Client *client)
{
    std::vector<Client*>::iterator it = std::find(_listInvite.begin(), _listInvite.end(), client);
    if (it != _listInvite.end())
        _listInvite.erase(it);
}

int             Channel::isOperator(Client *client)
{
    if (client->GetClientFd() == _operator)
        return (1);
    return (0);
}

void            Channel::Broadcast(std::string message)
{
    std::vector<Client *>::const_iterator it;
    for (it = ChannelClient.begin(); it != ChannelClient.end(); ++it)
    {
        if ((*it) != NULL)
            sendMessage((*it)->GetClientFd() , message);   
    }
}

int             Channel::isUserInChannel(Client *clientToInvite)
{
    std::vector<Client *>::iterator it;
    for (it = ChannelClient.begin(); it != ChannelClient.end(); ++it)
    {
        if ((*it) == clientToInvite)
        {
            return (1);
        }
    }
    return (0);
}

int        Channel::CheckKey(std::string pwd)
{
    if (_key == pwd)
    {
        return (1);
    }
    return (0);
}

int     Channel::findUser(std::string name){
    std::vector<Client *>::iterator it;
    for (it = ChannelClient.begin(); it != ChannelClient.end(); ++it)
    {
        if ((*it)->GetNickname() == name)
        {
            return ((*it)->GetClientFd());
        }
    }
    return (-1);
}
