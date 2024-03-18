/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:52:55 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 22:23:00 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

# define STDOUT 1
# define STDERR 2
# define STDIN 0
# define MINISHELL "MINISHELL$ "
# define SUCCESS 0
# define FAILURE 1
# define ERROR_PREFIX "minishell: "
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '\"'
# define TOKENIZE_ERROR_NUMBER 258
# define PARSE_ERROR_NUMBER 258

typedef struct s_token		t_token;
enum						e_token_kind
{
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
};
typedef enum e_token_kind	t_token_kind;

enum						e_node_kind
{
	ND_PIPELINE,
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
};
typedef enum e_node_kind	t_node_kind;

typedef struct s_node
{
	t_token					*args;
	t_node_kind				kind;
	struct s_node			*next;
	struct s_node			*redirect;
	t_token					*filename;
	int						inpipe[2];
	int						outpipe[2];
	struct s_node			*command;
	int						targetfd;
	int						filefd;
	int						stashed_targetfd;
	t_token					*delimiter;
}							t_node;

struct						s_token
{
	char					*word;
	t_token_kind			kind;
	t_token					*next;
};

typedef struct s_var
{
	char					*key;
	char					*value;
	struct s_var			*next;
}							t_var;
typedef struct s_map
{
	t_var					item_head;
}							t_map;

typedef struct g_status
{
	bool					had_error;
	int						exit_status;
	int						signal;
	int						is_interrupted;
	t_map					*envmap;
}							t_status;

t_token						*tokenize(char *line);
void						fatal_error(const char *msg);
void						perror_prefix(void);
// t_token	*expand_token(t_token *tok);
bool						is_metacharacter(char c);
t_node						*parse(t_token *tok);
t_token						*new_token(char *word, t_token_kind kind);
void						show_node(t_node *node);
void						free_node(t_node *node);
void						free_token(t_token *tok);
void						parse_error(t_token *tok, t_token **tol_list);
void						tokenize_error(char *line, char **line_loc);
void						assert_error(const char *msg);
t_node						*redirect_out(t_token **tok_loc, t_token *tok);
t_node						*redirect_in(t_token **tok_loc, t_token *tok);
t_node						*redirect_append(t_token **tok_loc, t_token *tok);
t_node						*new_node(t_node_kind kind);
void						append_node(t_node **node_loc, t_node *node);
t_token						*tokdup(t_token *tok);
int							exec(t_node *node);
char						**get_environ(t_map *map);
size_t						get_sizeof_map(t_map *map);
char						*get_value(char *key);
void						execute_command(char **command_splitted,
								char **envp);
void						make_map(void);
char						*get_full_sentence(t_var *var);
int							builtin_export(char **argv);
int							add_var(t_map *map, char *line, bool null_value);
int							builtin_env(void);
int							builtin_unset(char **argv);
int							remove_var(t_map *map, char *key);
int							builtin_pwd(void);
int							builtin_echo(char **argv);
int							builtin_cd(char **argv);
void						setup_signal(void);
int							builtin_exit(char **argv);
t_node						*redirect_heredoc(t_token **tok_loc, t_token *tok);
t_node						*expand(t_node *node);
void						reset_signals(void);
int							get_filefd(t_node *node);

t_token						*word(char **line_loc, char *line);
void						write_user_input_to_pipe(char *delimiter,
								int pipefd[2]);
int							open_heredoc(char *delimiter);

size_t						get_sizeof_token(t_token *args);
char						**convert_to_argv(t_token *args);
void						free_argv(char **argv);
int							isbuiltin(t_node *command);
int							stash_fd(int fd);

void						set_pipe(t_node *node);
void						set_parent_pipe(t_node *node);
void						set_child_pipe(t_node *node);

void						reset_redirect(t_node *node);
int							is_redir_kind(t_node_kind kind);
void						dup_redirect(t_node *node);

void						free_node(t_node *node);
void						free_token(t_token *tok);

bool						is_op(t_token *tok, char *op);
bool						is_control_operator(t_token *tok);
void						append_token(t_token **tok_loc, t_token *tok);
void						append_node(t_node **node_loc, t_node *node);
t_node						*new_node(t_node_kind kind);
t_node						*_the_first_node(t_node_kind kind);

bool						is_space(char c);
bool						is_operator(const char *s);
bool						is_metacharacter(char c);
bool						is_word(const char *s);

void						update_oldpwd(char *pwd_value);
bool						detect_target(char **path_loc, char *path,
								char *target);
void						remove_last_dir(char *pwd_value);
void						add_dir(char *new_pwd, char *path, char **path_loc);

bool						does_overflow(long result, int flag, char *string,
								bool *is_valid);
long						ft_atol(char *string, bool *is_valid);
bool						is_only_digit(char *str);
int							fit_res_to_int(long res);
void						print_exit_error(char *arg, char *message);

int							add_var(t_map *map, char *line, bool null_value);
int							remove_var(t_map *map, char *key);
char						*get_value(char *key);
size_t						get_sizeof_map(t_map *map);
int							remake_map(t_map *map, char *key, char *value);
t_var						*new_var(char *key, char *value);

bool						is_alpha_under(char c);
bool						is_alpha_num_under(char c);
bool						is_identifier(const char *s);
bool						is_variable(char *s);
bool						is_macro(char *s);
void						remove_quote(t_node *node);
void						quote_removal(t_token *tok);
void						append_single_quote(char **dst, char **rest,
								char *p);
void						append_double_quote(char **dst, char **rest,
								char *p);
void						expand_variable_str(char **dst, char **rest,
								char *p);
void						append_char(char **s, char c);

int							token_test(t_token *tok);

#endif