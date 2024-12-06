#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

void SocketCreationAndServer()
{
    // Créer le socket
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer < 0)
    {
        throw std::runtime_error("Socket creation failed");
    }

    std::cout << "Socket created !" << std::endl;

    // Créer l'adresse serveur (écoute sur le port 6667)
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Accepter toutes les interfaces réseau
    serverAddress.sin_port = htons(6667);       // Port 6667

    // Lier le socket à l'adresse
    if (bind(socketServer, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        throw std::runtime_error("Bind failed");
    }

    std::cout << "Bind completed!" << std::endl;

    // Écouter les connexions entrantes
    if (listen(socketServer, 5) < 0)
    {
        throw std::runtime_error("Listen failed");
    }

    std::cout << "Listening for connections on port 6667..." << std::endl;

    // Accepter une connexion entrante
    int clientSocket = accept(socketServer, nullptr, nullptr);
    if (clientSocket < 0)
    {
        throw std::runtime_error("Accept failed");
    }

    std::cout << "Client connected!" << std::endl;

    // Envoyer une réponse au client
    const char *message = "Welcome to the server!\n";
    send(clientSocket, message, strlen(message), 0);

    // Fermer la connexion
    close(clientSocket);
    close(socketServer);
}

int main()
{
    try
    {
        SocketCreationAndServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
