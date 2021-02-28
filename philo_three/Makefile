CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./includes -pthread
NAME = philo_three
INC = -I./includes
SRCS_DIR = ./srcs

SRCS = \
		utils.c \
		init.c \
		thread_funs.c \
		main.c

SRC_LIST = \
			$(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS = $(SRC_LIST:%.c=%.o)

all : $(NAME)

$(NAME) : $(OBJS) $(SRC_LIST)
	$(CC) $(CFLAGS) $(INC) $(OBJS) -o $(NAME)

clean :
	rm -rf $(OBJS)

re : fclean all

fclean : clean
	rm -rf $(NAME)

.PHONY: all clean fclean re