#include "Client.hpp"

Client::Client(int client_fd) : clientFd(client_fd), setNick(false), setUser(false), flagWelcome(0)
{
    nickname = "Anonymous";
    username = "Unknow";
}

std::string    Client::GetNickname()
{
    return (nickname);
}

std::string    Client::GetUsername()
{
    return (username);
}

int            Client::GetClientFd()
{
    return (clientFd);
}


void    Client::SetNickname(std::string newname)
{
    std::string oldNickName = nickname;
    nickname = newname;
    setNick = true;
    sendMessage(clientFd, ":" + oldNickName + " NICK " + nickname + "\r\n");
    std::cout << "\033[1;34m[D] Nick set\033[0m" << std::endl;

}

void    Client::SetUsername(std::string firstname, std::string lastname)
{
    username = firstname.substr(1, firstname.size()) + lastname;
    setUser = true;
    std::cout << "\033[1;34m[D] User set\033[0m" << std::endl;
}



std::string Client::recvMessage()
{
    char buffer[1024] = {0};
    ssize_t bytes_read = read(clientFd, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0)
        return ("");

    std::string message(buffer);
    return (message);
}

int    Client::Authentication(std::string password, int sizeTab, std::string element)
{
    std::string Pass = sizeTab > 1 ? element : "";

    if (Pass == password)
    {
        std::cout << "\033[1;34m[D] Authenticated\033[0m" << std::endl;
        return (1);
    }
    sendMessage(clientFd, "464 :Password incorrect\r\n");
    return (0);
}


void    Client::Ping(std::string token)
{
    std::string response = "PONG " + token + "\r\n";
    sendMessage(clientFd, response);
}

void    Client::PrivMsg(const std::map<int, Client *> &ClientsList, const std::vector<Channel *> &ChannelList, std::string target, std::string message)
{
    if (target.empty() || message.empty())
    {
        std::string errorMsg = "461 PRIVMSG :Not enough parameters\r\n";
        sendMessage(clientFd, errorMsg);
        return ;
    }

    for (size_t i = 0; i < ChannelList.size(); ++i)
    {
        Channel *channel = ChannelList[i];
        if (channel->getNameChannel() == target)
        {
            std::vector<Client *> members = channel->getClients();
            for (size_t j = 0; j < members.size(); ++j)
            {
                Client *member = members[j];
                if (member != this)
                {
                    std::string channelMsg = ":" + nickname + " PRIVMSG " + target + " :" + message + "\r\n";
                    sendMessage(member->GetClientFd(), channelMsg);
                }
            }
            return ;
        }
    }

    std::map<int, Client *>::const_iterator it;
    for (it = ClientsList.begin(); it != ClientsList.end(); ++it)
    {
        int clientID = it->first;
        Client *client = it->second;

        if (client != NULL && client->GetNickname() == target)
        {
            std::string privMsg = ":" + nickname + " PRIVMSG " + target + " :" + message + "\r\n";
            sendMessage(clientID, privMsg);
            return;
        }
    }
    std::cout << "Error: No client with nickname '" << target << "' found." << std::endl;
    std::string errorMsg = "401 " + target + " :No such nick/channel\r\n";
    sendMessage(clientFd, errorMsg);
}

void    Client::SetMode(std::vector<std::vector<std::string> > av, int i, std::vector<Channel *> ChannelList)
{
    Channel *channel = NULL;
    std::string mode;
    std::string channelName = av[i][1];
    std::string parameter;
    for (std::vector<Channel *>::iterator it = ChannelList.begin(); it != ChannelList.end(); ++it)
    {
        if ((*it)->getNameChannel() == channelName)
        {
            channel = *it;
            break;
        }
    }
    if (channel == NULL)
    {
        sendMessage(clientFd, "403 " + channelName + " :No such channel\r\n");
        return ;
    }

    if (av[i].size() > 2 && !av[i][2].empty())
    {
        mode = av[i][2];
    }
    else
    {
        mode = "";
    }
    if (av[i].size() > 3 && !av[i][3].empty())
    {
        parameter = av[i][3];
    }
    else
    {
        parameter = "";
    }
    
    if (!channel->isOperator(this))
    {
        sendMessage(clientFd, "482 " + nickname + " " + channelName + " :You're not a channel operator\r\n");
        return ;
    }

    if (mode == "-i")
    {
        if (channel->getInviteOnly() == true)
        {
            std::cout << "\033[1;33m{ MODE } -i Active :\033[0m Channel ouvert au public !" << std::endl;
            channel->setInvitOnly(false);
            std::string modeMessage = "MODE " + channelName + " " + mode + "\r\n";
            channel->Broadcast(modeMessage);
        }
        return ;
    }
    if (mode == "+i")
    {
        if (channel->getInviteOnly() == false)
        {
            std::cout << "\033[1;33m{ MODE } -i Active :\033[0m Channel sous invitation uniquement !" << std::endl;
            channel->setInvitOnly(true);
            std::string modeMessage = "MODE " + channelName + " " + mode + "\r\n";
            channel->Broadcast(modeMessage);
        }
        return ;
    }

    if (mode == "+t")
    {
        if(channel->gettopicRestrict() == true)
        {
            channel->settopicRestrict(false);       
            std::string modeMessage = "MODE " + channelName + " " + mode + "\r\n";
            channel->Broadcast(modeMessage);
        }
        return;
    }
    if (mode == "-t")
    {
        if(channel->gettopicRestrict() == false)
        {
            channel->settopicRestrict(true);
            std::string modeMessage = "MODE " + channelName + " " + mode + "\r\n";
            channel->Broadcast(modeMessage);
        }
        return;
    }
    if (mode == "+k")
    {
        if (channel->getKey() != "Unknow")
        {
            std::string errorMsg = ":OttoIrc42 467 " + nickname + " " + channelName + " :Channel key already set\r\n";
            sendMessage(clientFd, errorMsg);
        }
        else
        {
            channel->setKey(parameter);
            std::string modeMessage = "MODE " + channelName + " " + mode + " " + parameter + "\r\n";
            channel->Broadcast(modeMessage);
        }
        return ;
    }
    if (mode == "-k")
    {
        if (channel->getKey().empty())
        {
            std::string errorMsg = ":OttoIrc42 461 " + nickname + " " + channelName + " :No channel key is set\r\n";
            sendMessage(clientFd, errorMsg);
        }
        else
        {
            channel->setKey("");
            std::string modeMessage = "MODE " + channelName + " " + mode + "\r\n";
            channel->Broadcast(modeMessage);
        }
        return ;
    }
    
    if (mode == "+o")
    {
        if(av[i].size() == 4){
            int newop = channel->findUser(av[i][3]);
            if (newop != -1){
                channel->setOperator(newop,av[i][3]);
                std::string modeMessage = "MODE " + channelName + " " + av[i][3] + " has been promoted.\r\n";
                channel->Broadcast(modeMessage);
            }
            else{
                std::string errorMsg = ":MODE   401 user " + channelName + " " + av[i][3] +" :not found" + "\r\n";
                sendMessage(clientFd, errorMsg);
            }
            return;
        }
    }
    else
    {
        std::string errorMsg = ":OttoIrc42 472" + nickname + " " + channelName + " " + mode +" :is unknown mode char for " + channelName + "\r\n";
        sendMessage(clientFd, errorMsg);
    } 
}

void Client::JoinChannel(std::string nameChannel, std::vector<Channel *> &ChannelList, std::string password)
{
    if (nameChannel.empty())
    {
        return;
    }

    Channel *channelToJoin = NULL;

    if (nameChannel[0] != '#' && nameChannel[0] != '&') 
    {
        std::string errorMsg = ":OttoIrc42 476 " + nickname + " " + nameChannel + " :Invalid channel name\r\n";
        sendMessage(clientFd, errorMsg);
        return;
    }
   
    for (std::vector<Channel *>::iterator it = ChannelList.begin(); it != ChannelList.end(); ++it)
    {
        if ((*it)->getNameChannel() == nameChannel)
        {
            channelToJoin = *it;
            break;
        }
    }

    if (channelToJoin == NULL)
    {
        channelToJoin = new Channel(nameChannel);
        ChannelList.push_back(channelToJoin);
        channelToJoin->setOperator(clientFd, nickname);
    }

    if (channelToJoin->getInviteOnly() == true)
    {
        if (!channelToJoin->CheckKey(password))
        {
            std::string errorMsg = ":OttoIrc42 475 " + nickname + " " + nameChannel + " :Cannot join channel (invite-only or wrong password)\r\n";
            sendMessage(clientFd, errorMsg);
            return;
        }
    }

    channelToJoin->addUser(this);
    sendMessage(clientFd, ":" + nickname + "!" + username + "@oszwalbe JOIN :" + nameChannel + "\r\n");

    for (std::vector<Client *>::const_iterator it = channelToJoin->getClients().begin(); it != channelToJoin->getClients().end(); ++it) 
    {
        if (*it != this) 
        {
            sendMessage((*it)->clientFd, ":" + nickname + "!" + username + "@oszwalbe JOIN :" + nameChannel + "\r\n");
        }
    }
    
    std::string userList = ":";
    for (std::vector<Client *>::const_iterator it = channelToJoin->getClients().begin(); it != channelToJoin->getClients().end(); ++it) 
    {
        if ((*it)->clientFd == channelToJoin->getOperator()) 
            userList += "@" + (*it)->GetNickname() + " ";
        else 
            userList += (*it)->GetNickname() + " ";
    }
    if (!userList.empty()) 
        userList = userList.substr(0, userList.size() - 1);

    std::string nameReply = ":OttoIrc42 353 " + nickname + " = " + nameChannel + " " + userList + "\r\n";
    sendMessage(clientFd, nameReply);
    std::string endOfNames = ":OttoIrc42 366 " + nickname + " " + nameChannel + " :End of /NAMES list.\r\n";
    sendMessage(clientFd, endOfNames);

    if (!channelToJoin->getTopic().empty()) 
    {
        std::string topicMessage = ":OttoIrc42 332 " + nickname + " " + nameChannel + " :" + channelToJoin->getTopic() + "\r\n";
        sendMessage(clientFd, topicMessage);
    }
}

void            Client::Kick(std::string channelname, std::string user, const std::map<int, Client *> &ClientsList, std::vector<Channel *> ChannelList, std::string message)
{
    Channel *channel = NULL;
    for (std::vector<Channel *>::iterator it = ChannelList.begin(); it != ChannelList.end(); ++it)
    {
        if ((*it)->getNameChannel() == channelname)
        {
            channel = *it;
            break;
        }
    }

    if (channel == NULL)
    {
        sendMessage(clientFd, "403 " + channelname + " :No such channel\r\n");
        return;
    }
    if (!channel->isOperator(this))
    {
        sendMessage(clientFd, "482 " + nickname + " " + channelname + " :You're not a channel operator\r\n");
        return ;
    }

    Client* clientToKick = findClientByName(user, ClientsList);
    if (clientToKick == NULL)
    {
        sendMessage(clientFd, "401 " + user + " :No such nick/channel\r\n");
        return;
    }
    if (!channel->isUserInChannel(clientToKick))
    {
        sendMessage(clientFd, "442 " + user + " " + channelname + " :You're not on that channel\r\n");
        return;
    }

    channel->removeUser(clientToKick);
    
    std::string kickMessage = message.empty() ? "You have been kicked" : message;
    std::string notification = ":" + this->GetNickname() + " KICK " + channelname + " " + user + " :" + kickMessage + "\r\n";

    channel->Broadcast(notification);
    sendMessage(clientToKick->GetClientFd(), notification);
}

void            Client::Invite(std::string nameUser, std::string name, std::vector<Channel *> ChannelList, const std::map<int, Client *> &ClientsList)
{
    Channel* channel = NULL;
    for (std::vector<Channel *>::iterator it = ChannelList.begin(); it != ChannelList.end(); ++it)
    {
        if ((*it)->getNameChannel() != name)
        {
            channel = *it;
            break;
        }
    }
    if (!channel)
    {
        std::string errorMsg = ":OttoIrc42 403 " + GetNickname() + " " + name + " :No such channel\r\n";
        sendMessage(clientFd, errorMsg);
        return;
    }

    if (!channel->isOperator(this))
    {
        std::string errorMsg = ":OttoIrc42 482 " + GetNickname() + " " + name + " :You're not channel operator\r\n";
        sendMessage(clientFd, errorMsg);
        return;
    }

    Client* clientToInvite = findClientByName(nameUser, ClientsList);
    if (!clientToInvite)
    {
        std::string errorMsg = ":OttoIrc42 401 " + GetNickname() + " " + nameUser + " :No such nick\r\n";
        sendMessage(clientFd, errorMsg);
        return;
    }

    if (channel->isUserInChannel(clientToInvite))
    {
        std::string errorMsg = ":OttoIrc42 443 " + GetNickname() + " " + nameUser + " " + name + " :User already in channel\r\n";
        sendMessage(clientFd, errorMsg);
        return;
    }

    std::string inviteMessage = ":OttoIrc42 INVITE " + nameUser + " :" + name + "\r\n";
    sendMessage(clientToInvite->GetClientFd(), inviteMessage);

    std::string responseMessage = ":OttoIrc42 341 " + GetNickname() + " " + nameUser + " " + name + "\r\n";
    sendMessage(clientFd, responseMessage);
}

void            Client::SetTopic(std::string channel, std::vector<Channel *> ChannelList, std::string newTopic)
{
    Channel* foundChannel = NULL;
    for (std::vector<Channel*>::iterator it = ChannelList.begin(); it != ChannelList.end(); ++it)
    {
        if (*it != NULL && (*it)->getNameChannel() == channel)
        {
            foundChannel = *it;
            break;
        }
    }
    if (foundChannel == NULL)
    {
        std::string errorMsg = ":403 " + nickname + " " + channel + " :No such channel\r\n";
        sendMessage(clientFd, errorMsg);
        return;
    }

    if (newTopic.empty())
    {
        const std::string& currentTopic = foundChannel->getTopic();
        if (currentTopic.empty())
        {
            std::string msg = ":331 " + nickname + " " + channel + " :No topic is set\r\n";
            sendMessage(clientFd, msg);
        }
        else
        {
            std::string msg = ":332 " + nickname + " " + channel + " :" + currentTopic + "\r\n";
            sendMessage(clientFd, msg);
        }
        return ;
    }

    if (foundChannel->getOperator() != clientFd && foundChannel->gettopicRestrict() == false)
    {
        std::string errorMsg = "482 " + nickname + " " + channel + " :You're not a channel operator\r\n";
        sendMessage(clientFd, errorMsg);
        return;
    }
    foundChannel->setTopic(newTopic);
    std::string topicMessage = ":" + nickname + "!" + username + "@server TOPIC " + channel + " :" + newTopic + "\r\n";
    foundChannel->Broadcast(topicMessage);
}