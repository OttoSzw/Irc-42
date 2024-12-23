#include "Server.hpp"

int main(int ac, char **av)
{
    Server Instance;


    if (ac != 3)
    {
        std::cout << std::endl;
        std::cout << "\033[1;31m    Your output should be this way : ./Irc <port> <password>\033[0m " << std::endl;
        std::cout << std::endl;
        return (2);
    }

    try
    {
        Instance.assignPort(atoi(av[1]));
        Instance.assignPassword(av[2]);
        Instance.SocketCreationOfServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
