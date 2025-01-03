#include "Server.hpp"

Server::Server(int PortGiven, std::string PasswordGiven) : port(PortGiven), password(PasswordGiven), valid(0)
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
    epollFd = epoll_create1(0);
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
                newConnection();
            else
                handleConnection(events[i].data.fd);
        }
    }

    close(epollFd);
    close(socketServer);
}

void    Server::newConnection()
{
    int client_fd = accept(socketServer, 0, 0);
    if (client_fd < 0)
        throw std::runtime_error("Accept error");

    // Add new client to epoll
    struct epoll_event client_ev;
    client_ev.events = EPOLLIN | EPOLLET;
    client_ev.data.fd = client_fd;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, client_fd, &client_ev) < 0)
    {
        close(client_fd);
        throw std::runtime_error("Epollctl error");
    }

    ClientsList[client_fd] = new Client(client_fd);
    std::cout << "New client connected: FD " << client_fd << std::endl;
}

void Server::handleConnection(int client_fd)
{
    std::string message = ClientsList[client_fd]->recvMessage();
    std::vector<std::vector<std::string> > av = CommandSplitParam(message);

    if (av.size() == 0 || av[0].empty())
    {
        std::cout << "Client disconnected: FD " << client_fd << std::endl;
        close(client_fd);
        delete ClientsList[client_fd];
        ClientsList.erase(client_fd);
        epoll_ctl(epollFd, EPOLL_CTL_DEL, client_fd, 0);
    }

    for (size_t i = 0; i < av.size(); i++)
    {

        if (av[i][0] == "PASS" || av[i][0] == "NICK" || av[i][0] == "USER" || av[i][0] == "CAP")
        {
            if (av[i][0] == "PASS")
                valid = ClientsList[client_fd]->Authentication(password, av[i].size(), av[i][1]);
            if (av[i][0] == "NICK" && valid != 0)
                ClientsList[client_fd]->SetNickname(av[i][1]);
            if (av[i][0] == "USER" && valid != 0)
                ClientsList[client_fd]->SetUsername(av[i][4], av[i][5]);
        }
        if (ClientsList[client_fd]->GetNickname() != "Anonymous" && ClientsList[client_fd]->GetUsername() != "Unknow")
        {
            std::string welcomeMessage = "001 " + ClientsList[client_fd]->GetNickname() + " :Welcome to the IRC Network " +
                                        ClientsList[client_fd]->GetNickname() + "!" +
                                        ClientsList[client_fd]->GetUsername() + "@localhost\n";
            send(client_fd, welcomeMessage.c_str(), welcomeMessage.size(), 0);
        }
    }
    
}