#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <map>
#include "Utils.hpp"
#include "Commands.hpp"

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

typedef struct s_ClientData
{
	std::string nickname;
	std::string username;
	bool HasNickName;
	bool HasUserName;

	s_ClientData() : HasNickName(false), HasUserName(false) {}
} t_ClientData;


class Server
{
	private:

		int							 	port;
		std::string						password;

	public:

		int 			getPort();
		void			assignPort(int P);
		std::string		getPassword();
		void			assignPassword(char *P);

		void			SocketCreationOfServer();
	
};

#endif
