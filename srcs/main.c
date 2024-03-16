/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:03:52 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 19:04:15 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/libft.h"
# include "../includes/minishell.h" 

void minishell();

t_status status = {};
t_map *envmap;

void init_status(void)
{
	status.had_error = false;
	status.exit_status = 0;
	status.signal = 0;
	status.is_interrupted = false;
}

int main(int argc, char **argv)
{
	init_status();
	(void)argv;
	if (argc == 1)
		minishell();
	return (0);
}

void handle_error(char *msg)
{
	perror(msg);
	exit(-1);
}

void excecute_token(t_token *tok)
{
	extern char	**environ;
	char **command_splitted;
	int token_count = 0;

	t_token *tmp = tok;
	while (tmp)
	{
		token_count++;
		tmp = tmp->next;
	}
	command_splitted = malloc(sizeof(char *) * (token_count + 1));
	if (command_splitted == NULL)
		handle_error("malloc");
	for (int i = 0; i < token_count; i++)
	{
		command_splitted[i] = tok->word;
		tok = tok->next;
	}
	command_splitted[token_count] = NULL;
	execute_command(command_splitted, environ);
	exit(1);
}

void free_token(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->word)
		free(tok->word);
	free_token(tok->next);
	free(tok);
}

void free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_token(node->args);
	free_token(node->filename);
	free_node(node->redirect);
	free_node(node->next);
	free_node(node->command);
	free(node);
}

void interpret(char *line)
{
	t_token *tok;
	t_node  *node;

	status.had_error = false;
	tok = tokenize(line);
	if (tok->kind == TK_EOF)
		;
	else if(status.had_error)
	{
		status.exit_status = ERROR_IN_TOKENIZE;
	}
	else
	{
		node = parse(tok);
		if(status.had_error == true)
		{
			status.exit_status = ERROR_IN_PARSE;
		}
		else
		{
			expand(node);
			// show_node(node);
			status.exit_status = exec(node);
		}
		free_node(node);
	}
	free_token(tok);
}

void minishell()
{
	int		status;

	rl_outstream = stderr;
	status = 0;
	make_map();
	setup_signal();
	while(1)
	{
		char *line;
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		interpret(line);
		free(line);
	}
	exit(status);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }
