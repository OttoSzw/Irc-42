#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <fcntl.h>
#include <iostream>

void SuperSaiyan2(int socketFd);
std::string Trim(const std::string &str);
std::string ExtractPass(std::string passwordStr, const std::string &pass);

#endif