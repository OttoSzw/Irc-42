#include "Client.hpp"

Client::Client(int client_fd) : clientFd(client_fd), setNick(false), setUser(false)
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
    nickname = newname;
    setNick = true;
}

void    Client::SetUsername(std::string firstname, std::string lastname)
{
    username = firstname.substr(1, firstname.size()) + lastname;
    setUser = true;
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
        return (1);
    }
    return (0);
}

void    Client::Ping()
{
    
}