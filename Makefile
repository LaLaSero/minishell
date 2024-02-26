NAME = minishell
SRCS = srcs/main.c srcs/tokenize.c srcs/expand.c srcs/parse.c
SRCS += pipex/pipex.c pipex/execute_command.c srcs/test.c
SRCS += srcs/redirection.c srcs/error.c
# SRCS += get_next_line/get_next_line.c get_next_line/get_next_line_utils.c
INCLUDE = includes
CC = cc
CFLAGS = -lreadline
# -Wall -Wextra -Werror
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C libft

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

%.o: %.c $(INCLUDE)
	$(CC) -c $(CFLAGS) -o $@ $< -I $(INCLUDE)

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
