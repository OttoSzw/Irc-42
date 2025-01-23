NAME = ircserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -Iinc -std=c++98 -g3

SRCS =  src/Channel.cpp\
		src/Client.cpp\
		src/main.cpp\
		src/Server.cpp\
		src/Utils.cpp\
		# src/Bot.cpp\

HEADERS = inc/Channel.hpp\
		inc/Client.hpp\
		inc/Server.hpp\
		inc/Utils.hpp\
		# inc/Bot.hpp\

OBJDIR = objs

OBJS = $(SRCS:src/%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: src/%.cpp $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(objs)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: clean fclean re