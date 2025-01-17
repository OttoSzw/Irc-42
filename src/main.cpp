#include "Server.hpp"


int main(int ac, char **av)
{
    if ( ac != 3)
    {
        std::cerr << "Numbers of arguments need to be 3 !" << std::endl;
        return (2);
    }

    try
    {
        std::string trans(av[2]);
        Server Instance(atoi(av[1]), trans);

        Instance.RunningServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}