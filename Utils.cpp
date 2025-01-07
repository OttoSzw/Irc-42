#include "Utils.hpp"

std::string toUpper(const std::string &str)
{
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

std::vector<std::vector<std::string> > CommandSplitParam(const std::string &str)
{
    std::vector<std::vector<std::string> > result;
    std::istringstream iss(str);
    std::string line;

    // Séparer par saut de ligne (\n)
    while (std::getline(iss, line))
    {
        std::vector<std::string> tokens;
        std::istringstream lineStream(line);
        std::string token;

        // Séparer chaque ligne par espace
        while (lineStream >> token)
        {
            tokens.push_back(token);
        }

        // Mettre en majuscule le premier élément de chaque ligne
        if (!tokens.empty())
        {
            tokens[0] = toUpper(tokens[0]);
        }

        result.push_back(tokens);
    }

    return result;
}

void sendMessage(int client_fd, const std::string& message)
{
    send(client_fd, message.c_str(), message.size(), 0);
}


void sendPrivMessage(int client_fd, std::string from, std::string nameReceive, const std::string& message)
{   
    std::cout << "ID : " << client_fd << "FROM : " << from << "  NAMERECEIVE : " << nameReceive << std::endl;
    std::string privMsg = from + " PRIVMSG " + nameReceive + " :" + message + "\r\n";
    
    if (send(client_fd, privMsg.c_str(), privMsg.size(), 0) <= 0)
    {
        std::cout << "Ca marche pas" << std::endl;
    }
}


std::string mesgParsing(const std::vector<std::string> &av)
{
    std::string message;

    for (size_t i = 2; i < av.size(); i++)
    {
        if (!message.empty() && i != 2)
            message += " ";
        message += av[i];
    }
    return (message);
}