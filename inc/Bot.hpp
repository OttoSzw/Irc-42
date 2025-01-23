#ifndef BOT_HPP
#define BOT_HPP 

#include <iostream>

class Bot
{
    private:

        std::string     name;
        std::string     type;
        int             time;

    public:

        Bot();

        void    Bot::sendMsg();

};

#endif