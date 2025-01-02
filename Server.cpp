#include "Server.hpp"

Server::Server(int PortGiven, std::string PasswordGiven) : port(PortGiven), password(PasswordGiven)
{
    socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer < 0)
        throw std::runtime_error("Socket creation failed");

    int optval = 1;
    if (setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw std::runtime_error("Setting socket option SO_REUSEADDR failed");
    
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(socketServer, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        throw std::runtime_error("Bind failed");

    if (listen(socketServer, SOMAXCONN) < 0)
        throw std::runtime_error("Listen failed");
}

void    Server::RunningServer()
{
    int epollFd = epoll_create1(0);
    if (epollFd < 0)
        throw std::runtime_error("Error epoll failed Instance");
    
    struct epoll_event ev, events[10];
    ev.events = EPOLLIN;
    ev.data.fd = socketServer;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketServer, &ev) < 0)
        throw std::runtime_error("Failed to add socketServer to epoll"); 
    
    while (true)
    {
        int newEvents = epoll_wait(epollFd, events, 10, -1);
        if (newEvents < 0)
        {
            perror("epoll_wait");
            break;
        }
        for (int i = 0; i < newEvents; ++i)
        {
            if (events[i].data.fd == socketServer)
                newConnection(epollFd);
            else
                handleConnection(epollFd, events, i);
        }
    }

    close(epollFd);
    close(socketServer);
}

void    Server::newConnection(int epollFd)
{
    int client_fd = accept(socketServer, 0, 0);
    if (client_fd < 0)
        throw std::runtime_error("Accept error");

    // Add new client to epoll
    struct epoll_event client_ev;
    client_ev.events = EPOLLIN | EPOLLET; // Read events + edge-triggered
    client_ev.data.fd = client_fd;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, client_fd, &client_ev) < 0)
    {
        close(client_fd);
        throw std::runtime_error("Epollctl error");
    }

    ClientsList[client_fd] = new Client;
    std::cout << "New client connected: FD " << client_fd << std::endl;
}

void    Server::handleConnection(int epollFd, epoll_event *events, int i)
{
    char buffer[1024] = {0};
    int client_fd = events[i].data.fd;
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0)
    {
        if (bytes_read == 0)
            std::cout << "Client disconnected: FD " << client_fd << std::endl;
        else
            perror("read");

        close(client_fd);
        delete ClientsList[client_fd];
        ClientsList.erase(client_fd);
        epoll_ctl(epollFd, EPOLL_CTL_DEL, client_fd, 0);
    }
    else
    {
        std::cout << "Message from FD " << client_fd << ": " << buffer << std::endl;
        write(client_fd, buffer, bytes_read);
    }
}