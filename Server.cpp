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

void Server::SocketCreationAndServer()
{
    // Créer le socket
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer < 0)
    {
        throw std::runtime_error("Socket creation failed");
    }

    std::cout << "Socket created !" << std::endl;

    // Autoriser la réutilisation de l'adresse/port
    int optval = 1;
    if (setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        throw std::runtime_error("Setting socket option SO_REUSEADDR failed");
    }

    std::cout << "Socket option SO_REUSEADDR set!" << std::endl;

    // Créer l'adresse serveur
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Accepter toutes les interfaces réseau
    serverAddress.sin_port = htons(port);       // Port

    std::cout << "Trying to bind to port: " << port << std::endl;

    // Lier le socket à l'adresse
    if (bind(socketServer, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Bind failed");
        throw std::runtime_error("Bind failed");
    }

    std::cout << "Bind completed!" << std::endl;

    // Écouter les connexions entrantes
    if (listen(socketServer, 5) < 0)
    {
        throw std::runtime_error("Listen failed");
    }

    std::cout << "Listening for connections on port " << port << "..." << std::endl;

    // Accepter une connexion entrante
    int clientSocket = accept(socketServer, NULL, NULL);
    if (clientSocket < 0)
    {
        throw std::runtime_error("Accept failed");
    }

    std::cout << "Client connected!" << std::endl;

    // Demander au client de fournir un mot de passe
    const char *requestMessage = "Please enter the password: ";
    send(clientSocket, requestMessage, strlen(requestMessage), 0);

    // Buffer pour recevoir le mot de passe du client
    char buffer[1024];  
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0)
    {
        throw std::runtime_error("Error receiving data from client.");
    }

    buffer[bytesReceived] = '\0';  // Assurez-vous que la chaîne est bien terminée

    // Nettoyer le mot de passe reçu pour enlever les caractères indésirables
    trimPassword(buffer);

    // Vérifier si le mot de passe est correct
    if (std::string(buffer) != password)
    {
        const char *incorrectPasswordMessage = "Incorrect password! Closing connection.\n";
        send(clientSocket, incorrectPasswordMessage, strlen(incorrectPasswordMessage), 0);
        std::cout << "Client provided incorrect password. Closing connection." << std::endl;
        close(clientSocket);
        close(socketServer);
        return;  // Fermer la connexion si le mot de passe est incorrect
    }

    // Si le mot de passe est correct, continuer la communication avec le client
    const char *welcomeMessage = "Password correct! You are now connected.\n";
    send(clientSocket, welcomeMessage, strlen(welcomeMessage), 0);

    // Ensuite, tu peux ajouter le code pour gérer la communication normale avec le client

    // Fermer la connexion
    close(clientSocket);
    close(socketServer);
}
