/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:52:55 by yutakagi          #+#    #+#             */
/*   Updated: 2024/02/26 15:37:44 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdint.h>
# include <errno.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <stddef.h>
# include <stdlib.h>

# define STDOUT 1
# define STDERR 2
# define STDIN 0
# define MINISHELL "MINISHELL$ "

typedef struct s_token		t_token;
enum e_token_kind {
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
};
typedef enum e_token_kind	t_token_kind;

enum e_node_kind {
	ND_PIPELINE,
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
};
typedef enum e_node_kind	t_node_kind;

typedef struct s_node	t_node;
struct s_node {
	t_token		*args;
	t_node_kind	kind;
	t_node		*next;
	t_node *redirect;
	t_token *filename;
	int inpipe[2];
	int outpipe[2];
	t_node *command;
	int targetfd;
};


// `word` is zero terminated string.
struct s_token {
	char			*word;
	t_token_kind	kind;
	t_token			*next;
};

typedef struct s_var	t_var;

struct s_var {
	char	*key;
	char	*value;
	t_var	*next;
};

typedef struct g_status {
	bool 	had_error;
	int		exit_status;
	
}	t_status;

typedef struct s_map	t_map;
struct s_map {
	t_var	item_head;
};

# ifndef SUCCESS
#  define SUCCESS 0
# endif

# ifndef FAILURE
#  define FAILURE 1
# endif

t_token	*tokenize(char *line);
void	fatal_error(const char *msg);
#define ERROR_PREFIX "minishell: "
void	perror_prefix(void);
t_token *expand_token(t_token *tok);
bool	is_metacharacter(char c);
t_node *parse(t_token *tok);
t_token	*new_token(char *word, t_token_kind kind);
void show_node(t_node *node);
void free_node(t_node *node);
void free_token(t_token *tok);
void parse_error(t_token *tok, t_token **tol_list);
void tokenize_error(char *line, char **line_loc);
void assert_error(const char *msg);
t_node	*redirect_out(t_token **tok_loc, t_token *tok);
t_node	*redirect_in(t_token **tok_loc, t_token *tok);
t_node *redirect_append(t_token **tok_loc, t_token *tok);
t_node *new_node(t_node_kind kind);
void append_node(t_node **node_loc, t_node *node);
t_token	*tokdup(t_token *tok);

#define SINGLE_QUOTE_CHAR '\''
#define DOUBLE_QUOTE_CHAR '\"'
#define ERROR_IN_PARSE 258
#define ERROR_IN_TOKENIZE 258
#endif