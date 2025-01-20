#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include "Client.hpp"

class Client;

class Channel;

std::string toUpper(const std::string &str);

std::vector<std::vector<std::string> > CommandSplitParam(const std::string &str);

void sendMessage(int client_fd, const std::string &message);

std::string mesgParsing(const std::vector<std::string> &av);

Client     *findClientByName(std::string nameUser, const std::map<int, Client *> &ClientsList);

#endif