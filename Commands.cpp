#include "Commands.hpp"

void    NickCommand(int clientSocket, std::string message, t_ClientData& client)
{
    std::string nickname = message.substr(5);
    if (!nickname.empty())
    {
        client.nickname = nickname;
        client.HasNickName = true;
        const char *nickMessage = "Nickname set successfully.\r\n";
        send(clientSocket, nickMessage, strlen(nickMessage), 0);
        std::cout << "Client " << client.nickname << " set their nickname to: " << client.nickname << std::endl;
    }
    else
    {
        const char *errorMessage = "Error: Nickname cannot be empty.\n";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }
}