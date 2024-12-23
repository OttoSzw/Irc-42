#include "Utils.hpp"

void SuperSaiyan2(int socketFd)
{
    if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Fcntl ERROR F_SETFL!");
}

std::string Trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos)
        return ("");
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
        return (str.substr(first, last - first + 1));
}

std::string    ExtractPass(std::string passwordStr, const std::string& pass)
{
    (void)pass;
    size_t pos = passwordStr.find("PASS");
    if (pos != std::string::npos)
    {
        std::string extractWord = passwordStr.substr(pos + 5, pass.size());
        extractWord = Trim(extractWord);
        return (extractWord);
    }
    return (NULL);
}


