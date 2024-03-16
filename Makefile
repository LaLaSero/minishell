NAME = minishell
SRCS = srcs/main.c srcs/tokenize.c srcs/expand.c srcs/parse.c
SRCS += srcs/execute_command.c srcs/test.c
SRCS += srcs/redirection.c srcs/error.c srcs/exec.c 
SRCS += srcs/signal/signal.c srcs/signal/signal_utils.c
SRCS += srcs/environ_map/environ_utils.c srcs/environ_map/map_utils.c srcs/environ_map/map_init.c
SRCS += srcs/builtins/cd_dir/builtin_cd.c srcs/builtins/cd_dir/cd_utils.c
SRCS += srcs/builtins/exit_dir/builtin_exit.c srcs/builtins/exit_dir/exit_utils.c
SRCS += srcs/builtins/builtin_export.c srcs/builtins/builtin_env.c srcs/builtins/builtin_unset.c
SRCS += srcs/builtins/builtin_pwd.c srcs/builtins/builtin_echo.c
INCLUDE = includes
CC = cc
READLINE_DIR = $(shell brew --prefix readline)
CFLAGS = -I$(INCLUDE) -I$(READLINE_DIR)/include -Wall -Wextra -Werror
LDFLAGS = -L$(READLINE_DIR)/lib -lreadline -Wall -Wextra -Werror -g -fsanitize=address
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
