/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:03:52 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/26 00:31:53 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../includes/minishell.h" 

int	g_signal;

static void	_init_params(int *status, int *error)
{
	*status = 0;
	*error = false;
}

static void	_detect_interrupt(int *status)
{
	if (g_signal == 256)
	{
		*status = 1;
		g_signal = 0;
	}
}

static void	_analyze(char *line, int *status, int *error, t_map *envmap)
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
			expand(node, status, error, envmap);
			*status = exec(node, *status, envmap);
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

	_init_params(&status, &error);
	rl_outstream = stderr;
	make_map(&envmap);
	setup_signal();
	while (1)
	{
		if (g_signal == SIGINT)
			g_signal = 0;
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		_analyze(line, &status, &error, envmap);
		free(line);
		if (g_signal == 256)
			_detect_interrupt(&status);
	}
	write(1, "exit\n", 5);
	exit(status);
}

int	main(int argc, char **argv)
{
	g_signal = 0;
	(void)argv;
	if (argc == 1)
		_minishell_loop();
	return (0);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }
