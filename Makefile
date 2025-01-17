NAME = ircserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -Iinc -std=c++98 -g3

SRCS =  src/Channel.cpp\
		src/Client.cpp\
		src/main.cpp\
		src/Server.cpp\
		src/Utils.cpp\

HEADERS = inc/Channel.hpp\
		inc/Client.hpp\
		inc/Server.hpp\
		inc/Utils.hpp\

OBJDIR = objs

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@mkdir -p $(OBJDIR)
	mv $(OBJS) $(OBJDIR)

%.o : %.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) -c $< -o $(<:.cpp=.o)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: clean fclean re