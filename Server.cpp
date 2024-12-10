#include "Server.hpp"

int Server::getPort()
{
	return (port);
}

std::string Server::getPassword()
{
	return (password);
}

void	Server::assignPort(int P)
{
	port = P;
}

void Server::assignPassword(char *P)
{
	password = P;
}

void trimPassword(char* password)
{
    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n')
	{
        password[len - 1] = '\0';
    }
    while (len > 0 && (password[len - 1] == ' ' || password[len - 1] == '\t'))
	{
        password[--len] = '\0';
    }
}

void Server::SocketCreationOfServer()
{
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);

    if (socketServer < 0)
        throw std::runtime_error("Socket creation failed");

    std::cout << "\033[1;32mSocket created \033[0m!" << std::endl;
    std::cout << std::endl;

    int optval = 1;
    if (setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw std::runtime_error("Setting socket option SO_REUSEADDR failed");

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    sleep(1);
    std::cout << "\033[1;30m    Trying to bind to port : \033[0m" << port << std::endl;

    std::cout << std::endl;

    if (bind(socketServer, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        throw std::runtime_error("Bind failed");
    }

    sleep(1);
    std::cout << "\033[1;32mBind completed \033[0m!" << std::endl;
    sleep(1);
    
    std::cout << std::endl;

    if (listen(socketServer, 5) < 0)
    {
        throw std::runtime_error("Listen failed");
    }

    std::cout << "\033[1;35m    Listening for connections on port " << port << "... \033[0m" << std::endl;

    std::cout << std::endl;

    int clientSocket = accept(socketServer, NULL, NULL);
    if (clientSocket < 0)
    {
        throw std::runtime_error("Accept failed");
    }

    std::cout << "\033[1;33mClient connected \033[0m!" << std::endl;

    const char *requestMessage = "Please enter the password: ";
    send(clientSocket, requestMessage, strlen(requestMessage), 0);

    char buffer[1024];  
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0)
    {
        throw std::runtime_error("Error receiving data from client.");
    }

    buffer[bytesReceived] = '\0';

    trimPassword(buffer);

    if (std::string(buffer) != password)
    {
        const char *incorrectPasswordMessage = "Incorrect password! Closing connection.\n";
        send(clientSocket, incorrectPasswordMessage, strlen(incorrectPasswordMessage), 0);
        std::cout << "Client provided incorrect password. Closing connection." << std::endl;
        close(clientSocket);
        close(socketServer);
        return ;
    }

    const char *welcomeMessage = "Password correct! You are now connected.\n";
    send(clientSocket, welcomeMessage, strlen(welcomeMessage), 0);
   
    std::cout << std::endl;
    std::cout << "\033[1;30m  Client authenticated successfully ✅. Ready to receive messages.\033[0m" << std::endl;

    while (true)
    {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0)
        {
            std::cout << "Client disconnected or an error occurred." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << std::endl;
        std::cout << "Message from client: " << buffer;

        // Optionally, send an acknowledgment back to the client
        const char *ackMessage = "Message received\n";
        send(clientSocket, ackMessage, strlen(ackMessage), 0);
    }

    
    close(clientSocket);
    close(socketServer);
}
