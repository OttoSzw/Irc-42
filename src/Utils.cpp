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

    while (std::getline(iss, line))
    {
        std::vector<std::string> tokens;
        std::istringstream lineStream(line);
        std::string token;

        while (lineStream >> token)
            tokens.push_back(token);

        if (!tokens.empty())
            tokens[0] = toUpper(tokens[0]);

        result.push_back(tokens);
    }

    return (result);
}

void sendMessage(int client_fd, const std::string& message)
{
    send(client_fd, message.c_str(), message.size(), 0);
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


Client* findClientByName(std::string nameUser, const std::map<int, Client *> &ClientsList)
{
    std::map<int, Client *>::const_iterator it;
    for (it = ClientsList.begin(); it != ClientsList.end(); ++it)
    {
        if (it->second->GetNickname() == nameUser)
        {
            return (it->second);
        }
    }
    return (NULL);
}
