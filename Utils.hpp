#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <sys/socket.h>

std::string toUpper(const std::string &str);

std::vector<std::vector<std::string> > CommandSplitParam(const std::string &str);

void sendMessage(int client_fd, const std::string &message);

std::string mesgParsing(const std::vector<std::string> &av);

#endif