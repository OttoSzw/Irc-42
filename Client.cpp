#include "Client.hpp"

Client::Client(int client_fd) : clientFd(client_fd), setNick(false), setUser(false), isInvisible(0), flagWelcome(0)
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
    return (0);
}


void    Client::Ping(std::string token)
{
    std::string response = "PONG " + token;
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
    std::string errorMsg = "401 " + nickname + " " + target + " :No such nick/channel\n";
    sendMessage(clientFd, errorMsg);
}

void    Client::SetMode(std::string mode)
{
    if (mode == "+i")
    {
        isInvisible = 1;
        std::string message = "221 " + nickname + " :+i\n";
        sendMessage(clientFd, message);
    }
    else
    {
        std::string errorMsg = "472 " + nickname + " :Unknown MODE flag\n";
        sendMessage(clientFd, errorMsg);
    }
}

void Client::JoinChannel(std::string nameChannel, std::vector<Channel *> &ChannelList)
{
    if (nameChannel.empty())
    {
        return;
    }

    Channel *channelToJoin = NULL;

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
    }

    channelToJoin->addUser(this);
}
