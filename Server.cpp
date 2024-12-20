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


//utils for my server (type parse)


void SuperSaiyan2(int socketFd)
{
    int flags = fcntl(socketFd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl (F_GETFL)");
        exit(EXIT_FAILURE);
    }

    if (fcntl(socketFd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl (F_SETFL)");
        exit(EXIT_FAILURE);
    }
}

std::string    ExtractPass(std::string passwordStr, const std::string& pass)
{
    std::string extractWord = passwordStr.substr(pass.size());
    std::cout << "Mot extrait : " << extractWord << std::endl;
    return (extractWord);
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

void    handle_new_connection(int socketServer, int epollFd, const std::string& password, Client& data)
{
    int clientSocket = accept(socketServer, NULL, NULL);
    if (clientSocket < 0)
        throw std::runtime_error("Accept failed");

    const char *requestPasswordMessage = "Entrez votre mot de passe avec PASS: ";
    send(clientSocket, requestPasswordMessage, strlen(requestPasswordMessage), 0);

    char passwordBuffer[256];
    size_t bytesReceived = recv(clientSocket, passwordBuffer, sizeof(passwordBuffer) - 1, 0);
    if (bytesReceived <= 0)
    {
        std::cout << "Erreur lors de la réception du mot de passe." << std::endl;
        close(clientSocket);
    }

    passwordBuffer[bytesReceived] = '\0';
    std::string passwordStr(passwordBuffer);

    if (passwordStr.find("PASS") == 0)
    {
        passwordStr = ExtractPass(passwordStr, password);

        std::cout << "Mot de passe reçu: '" << passwordStr << "'" << std::endl;  // Debugging
        std::cout << "Mot de passe : '" << password << "'" << std::endl;  // Debugging

        if (passwordStr == password)
        {
            const char *WelcomeClientMessage = "\033[1;33mBienvenue sur le Server ! Amuse toi bien :3\n\033[0m";
            send(clientSocket, WelcomeClientMessage, strlen(WelcomeClientMessage), 0);

            data.clientNicknames[clientSocket] = "Anonymous";
            std::cout << "\033[1;33m   ✅ Client connected \033[0m" << data.clientNicknames[clientSocket] << std::endl;                  
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

void    handle_client_message(int clientSocket, Client& data)
{
    char buffer[BUFFER_SIZE];
    size_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0)
    {
        std::cout << "Client disconnected or an error occurred." << std::endl;
        close(clientSocket);
    }

    buffer[bytesReceived] = '\0';
    std::cout << "\033[1;35m    Message from client \033[0m" << data.clientNicknames[clientSocket] << ": " << buffer;

    const char *ackMessage = "\033[1;32mMessage received\033[0m\n";
    send(clientSocket, ackMessage, strlen(ackMessage), 0);
}

//server en marche

void Server::SocketCreationOfServer()
{
    Client data;

    int socketServer = initServer(port);

    SuperSaiyan2(socketServer);

    int epollFd = initEpoll(socketServer);

    std::cout << "\033[1;25m    Listening for connections on port " << port << "... \033[0m" << std::endl;
    std::cout << std::endl;

    epoll_event events[MAX_EVENTS];

    while (1)
    {
        int n = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (n == -1)
            throw std::runtime_error("EpollWait Problem");

        for (int i = 0; i < n; i++)
        {
            if (events[i].data.fd == socketServer)
                handle_new_connection(socketServer, epollFd, password, data);
            else
                handle_client_message(events[i].data.fd, data);
        }
    }    
}