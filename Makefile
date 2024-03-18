NAME = minishell
SRCS = srcs/main.c
SRCS += srcs/expand/expand.c srcs/expand/expand_utils_is.c srcs/expand/expand_utils_quote.c srcs/expand/expand_remove.c
SRCS += srcs/tokenize/tokenize.c srcs/tokenize/tokenize_utils.c srcs/tokenize/tokenize_quote.c
SRCS += srcs/parse/parse.c srcs/parse/parse_utils.c srcs/parse/parse_append_new.c
SRCS += srcs/test.c
SRCS += srcs/exec/exec.c srcs/exec/execute_command.c srcs/exec/exec_utils.c srcs/exec/exec_redirect.c
SRCS += srcs/exec/exec_pipe.c srcs/exec/exec_heredoc.c srcs/exec/make_argv.c srcs/exec/exec_filefd.c
SRCS += srcs/redirect/redirection.c
SRCS += srcs/error/error.c
SRCS += srcs/frees/frees.c
SRCS += srcs/signal/signal.c srcs/signal/signal_utils.c
SRCS += srcs/environ_map/environ_utils.c srcs/environ_map/map_utils.c srcs/environ_map/map_init.c
SRCS += srcs/environ_map/remake_map.c
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
