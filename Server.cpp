#include "Server.hpp"

// Mes getters et setters


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

//Init my server :)


int initServer(int port)
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
    std::cout << "\033[1;25m    Trying to bind to port : \033[0m" << port << std::endl;

    std::cout << std::endl;

    if (bind(socketServer, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        throw std::runtime_error("Bind failed");
    sleep(1);
    std::cout << "\033[1;32mBind completed \033[0m!" << std::endl;
    sleep(1);
    std::cout << std::endl;

    if (listen(socketServer, SOMAXCONN) < 0)
        throw std::runtime_error("Listen failed");

    return (socketServer);
}

// Init epoll

int initEpoll(int socketServer)
{
    int epollFd = epoll_create1(0);
    if (epollFd == -1)
        throw std::runtime_error("Epoll failed");

    epoll_event event = {};
    event.events = EPOLLIN;
    event.data.fd = socketServer;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketServer, &event) == -1)
        throw std::runtime_error("Erreur epoll_ctl (serveur)");
    
    return (epollFd);
}

//Gerer mes nouvelles connections

void    handle_new_connection(int socketServer, int epollFd, const std::string& password, std::map<int, ClientData>& data)
{
    int clientSocket = accept(socketServer, NULL, NULL);
    if (clientSocket < 0)
        throw std::runtime_error("Accept failed");

    char passwordBuffer[256];
    size_t bytesReceived = recv(clientSocket, passwordBuffer, sizeof(passwordBuffer) - 1, 0);
    if (bytesReceived <= 0)
    {
        std::cout << "Erreur lors de la réception du mot de passe." << std::endl;
        close(clientSocket);
    }

    passwordBuffer[bytesReceived] = '\0';
    std::string passwordStr(passwordBuffer);

    std::cout << "mot de pass : " << passwordStr << std::endl;

    if (passwordStr.find("PASS") != std::string::npos)
    {
        passwordStr = ExtractPass(passwordStr, password);

        if (passwordStr == password)
        {
            data[clientSocket].nickname = "Anonymous";
            data[clientSocket].username = "Unknow";
            std::cout << "\033[1;33m   ✅ Client connected \033[0m" << data[clientSocket].nickname << std::endl;
            std::cout << std::endl;
            SuperSaiyan2(clientSocket);

            epoll_event clientEvent = {};
            clientEvent.events = EPOLLIN;
            clientEvent.data.fd = clientSocket;

            if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &clientEvent) == -1)
            {
                perror("Erreur epoll_ctl (client)");
                close(clientSocket);
            }
            const char *rpl_welcome = ":server 001 Anonymous :Welcome to the IRC server, Anonymous! (Unknow@server)\r\n";
            send(clientSocket, rpl_welcome, strlen(rpl_welcome), 0);

            const char *rpl_yourhost = ":server 002 Anonymous :Your host is server, running version 1.0\r\n";
            send(clientSocket, rpl_yourhost, strlen(rpl_yourhost), 0);

            const char *rpl_created = ":server 003 Anonymous :This server was created on Mon Dec 23 2024\r\n";
            send(clientSocket, rpl_created, strlen(rpl_created), 0);

            const char *rpl_myinfo = ":server 004 Anonymous server 1.0 o o\r\n";
            send(clientSocket, rpl_myinfo, strlen(rpl_myinfo), 0);
        }
        else
        {
            const char *incorrectPasswordMessage = "\033[1;31mMot de passe incorrect. Connexion fermée.\033[0m\n";
            send(clientSocket, incorrectPasswordMessage, strlen(incorrectPasswordMessage), 0);
            close(clientSocket);
        }
    }
}

//gerer les messages clients existants

void handle_client_message(int clientSocket, std::map<int, ClientData>& data)
{
    char buffer[BUFFER_SIZE];
    size_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0)
    {
        std::cout << "Client disconnected or an error occurred." << std::endl;
        close(clientSocket);
        return;  // Sortir immédiatement après la déconnexion ou erreur
    }

    buffer[bytesReceived] = '\0';
    std::string message(buffer);

    std::map<int, t_ClientData>::iterator it = data.find(clientSocket);
    if (it != data.end())
    {
        t_ClientData& client = it->second;

        std::cout << "'" << message << "'" << std::endl;
        if (message.find("NICK") == 0)
        {
            NickCommand(clientSocket, message, client);
        }
        else if (message.find("USER") == 0 || message.find("userhost") == 0)
        {
            std::string user = message.substr(5);
            if (!user.empty())
            {
                client.username = user;
                client.HasUserName = true;
                const char *userMessage = "User set successfully.\n";
                send(clientSocket, userMessage, strlen(userMessage), 0);
                std::cout << "Client " << clientSocket << " set their user to: " << client.username << std::endl;
            }
            else
            {
                const char *errorMessage = "Error: User cannot be empty.\n";
                send(clientSocket, errorMessage, strlen(errorMessage), 0);
            }
        }
        else if (client.HasNickName && client.HasUserName)
            std::cout << "Message from " << client.nickname << " (" << client.username << "): " << message << std::endl;
    }
}

//server en marche

void Server::SocketCreationOfServer()
{

    int socketServer = initServer(port);

    SuperSaiyan2(socketServer);

    int epollFd = initEpoll(socketServer);

    std::cout << "\033[1;25m    Listening for connections on port " << port << "... \033[0m" << std::endl;
    std::cout << std::endl;

    epoll_event events[MAX_EVENTS];
    std::map<int, ClientData> clients;

    while (1)
    {
        int n = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (n == -1)
            throw std::runtime_error("EpollWait Problem");

        for (int i = 0; i < n; i++)
        {
            if (events[i].data.fd == socketServer)
                handle_new_connection(socketServer, epollFd, password, clients);
            else
                handle_client_message(events[i].data.fd, clients);
        }
    }    
}

