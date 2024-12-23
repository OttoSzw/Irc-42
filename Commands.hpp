#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include "Server.hpp"

void    NickCommand(int clientSocket, std::string message, t_ClientData& client);

#endif