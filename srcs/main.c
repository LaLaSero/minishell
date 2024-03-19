/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:03:52 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 13:55:24 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../includes/minishell.h" 

t_status	g_status;

static void	_init_status(void)
{
	g_status.signal = 0;
	g_status.is_interrupted = false;
	g_status.envmap = NULL;
}

static void	_analyze(char *line, int *status, int *error)
{
	t_token	*tok;
	t_node	*node;

	*error = false;
	tok = tokenize(line, error);
	if (tok->kind == TK_EOF)
		;
	else if (*error == true)
		*status = TOKENIZE_ERROR_NUMBER;
	else
	{
		node = parse(tok, error);
		if (*error == true)
			*status = PARSE_ERROR_NUMBER;
		else
		{
			expand(node, status, error);
			*status = exec(node, *status);
		}
		free_node(node);
	}
	free_token(tok);
}

static void	_minishell_loop(void)
{
	char	*line;
	int		status;
	int		error;
	t_map	*envmap;

	status = 0;
	error = false;
	rl_outstream = stderr;
	make_map();
	setup_signal();
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		_analyze(line, &status, &error);
		free(line);
	}
	write(1, "exit\n", 5);
	exit(status);
}

int	main(int argc, char **argv)
{
	_init_status();
	(void)argv;
	if (argc == 1)
		_minishell_loop();
	return (0);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }
