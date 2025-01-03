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