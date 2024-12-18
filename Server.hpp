#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "Client.hpp"
#include <map>
#include <string>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024


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
