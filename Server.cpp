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

void Server::SocketCreationOfServer()
{
    Client data;
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
    {
        throw std::runtime_error("Listen failed");
    }

    SuperSaiyan2(socketServer);

    int epollFd = epoll_create1(0);
    if (epollFd == -1)
    {
        throw std::runtime_error("Epoll failed");
    }

    epoll_event event = {};
    event.events = EPOLLIN;
    event.data.fd = socketServer;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketServer, &event) == -1)
    {
        throw std::runtime_error("Erreur epoll_ctl (serveur)");
    }

    std::cout << "\033[1;25m    Listening for connections on port " << port << "... \033[0m" << std::endl;
    std::cout << std::endl;

    epoll_event events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];

    while (1)
    {
        int n = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (n == -1)
        {
            throw std::runtime_error("EpollWait Problem");
        }

        for (int i = 0; i < n; i++)
        {
            if (events[i].data.fd == socketServer)
            {
                int clientSocket = accept(socketServer, NULL, NULL);
                if (clientSocket < 0)
                {
                    throw std::runtime_error("Accept failed");
                }

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
                size_t bytesReceived = recv(events[i].data.fd, buffer, sizeof(buffer) - 1, 0);
                if (bytesReceived <= 0)
                {
                    std::cout << "Client disconnected or an error occurred." << std::endl;
                    close(events[i].data.fd);
                    break;
                }

                buffer[bytesReceived] = '\0';
                std::cout << "\033[1;35m    Message from client \033[0m" << data.clientNicknames[events[i].data.fd] << ": " << buffer;

                const char *ackMessage = "\033[1;32mMessage received\033[0m\n";
                send(events[i].data.fd, ackMessage, strlen(ackMessage), 0);
            }
        }
    }    
}