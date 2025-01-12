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

void    Client::SetNickname(std::string newname)
{
    std::string oldNickName = nickname;
    std::cout << "OLD NICK : " << oldNickName << std::endl;
    nickname = newname;
    std::cout << "NEW NICK : " << nickname << std::endl;
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


void    Client::PrivMsg(const std::map<int, Client *> &ClientsList, std::string target, std::string message)
{
    std::map<int, Client *>::const_iterator it;
    for (it = ClientsList.begin(); it != ClientsList.end(); ++it)
    {
        int clientID = it->first;
        Client *client = it->second;

        if (client != NULL && client->GetNickname() == target)
        {
            sendPrivMessage(clientID, nickname, client->GetNickname(), message);
            std::cout << "Message sent to client " << target << " (ID: " << clientID << "): " << message << std::endl;
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