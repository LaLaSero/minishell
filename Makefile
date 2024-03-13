# NAME = minishell
# SRCS = srcs/main.c srcs/tokenize.c srcs/expand.c srcs/parse.c
# SRCS += pipex/pipex.c pipex/execute_command.c srcs/test.c
# SRCS += srcs/redirection.c srcs/error.c srcs/exec.c srcs/environ.c srcs/signal.c
# SRCS += srcs/builtins/builtin_cd.c
# SRCS += srcs/builtins/builtin_export.c srcs/builtins/builtin_env.c srcs/builtins/builtin_unset.c 
# SRCS += srcs/builtins/builtin_pwd.c srcs/builtins/builtin_echo.c
# INCLUDE = includes
# CC = cc
# CFLAGS = -lreadline
# # -Wall -Wextra -Werror
# OBJS = $(SRCS:.c=.o)
# LIBFT = libft/libft.a

# all: $(LIBFT) $(NAME)

# $(LIBFT):
# 	make -C libft

# $(NAME): $(OBJS)
# 	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

# %.o: %.c $(INCLUDE)
# 	$(CC) -c $(CFLAGS) -o $@ $< -I $(INCLUDE)

# clean:
# 	rm -f $(OBJS)
# 	make -C libft clean

# fclean: clean
# 	rm -f $(NAME)
# 	make -C libft fclean

# re: fclean all

# .PHONY: all clean fclean re
NAME = minishell
SRCS = srcs/main.c srcs/tokenize.c srcs/expand.c srcs/parse.c
SRCS += pipex/pipex.c pipex/execute_command.c srcs/test.c
SRCS += srcs/redirection.c srcs/error.c srcs/exec.c srcs/environ.c srcs/signal.c
SRCS += srcs/builtins/builtin_cd.c
SRCS += srcs/builtins/builtin_export.c srcs/builtins/builtin_env.c srcs/builtins/builtin_unset.c
SRCS += srcs/builtins/builtin_pwd.c srcs/builtins/builtin_echo.c
INCLUDE = includes
CC = gcc
READLINE_DIR = $(shell brew --prefix readline)
CFLAGS = -I$(INCLUDE) -I$(READLINE_DIR)/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C libft

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
