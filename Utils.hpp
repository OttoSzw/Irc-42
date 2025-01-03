#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

std::string toUpper(const std::string &str);

std::vector<std::vector<std::string> > CommandSplitParam(const std::string &str);

#endif