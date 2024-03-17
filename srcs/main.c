/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:03:52 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/17 18:13:27 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/libft.h"
# include "../includes/minishell.h" 

t_status g_status = {};
t_map *envmap;

static void	_init_status(void)
{
	g_status.had_error = false;
	g_status.exit_status = 0;
	g_status.signal = 0;
	g_status.is_interrupted = false;
}

static void	_analyze(char *line)
{
	t_token	*tok;
	t_node	*node;

	g_status.had_error = false;
	tok = tokenize(line);
	if (tok->kind == TK_EOF)
		;
	else if(g_status.had_error)
		g_status.exit_status = TOKENIZE_ERROR_NUMBER;
	else
	{
		node = parse(tok);
		if(g_status.had_error == true)
			g_status.exit_status = PARSE_ERROR_NUMBER;
		else
		{
			expand(node);
			// show_node(node);
			g_status.exit_status = exec(node);
		}
		free_node(node);
	}
	free_token(tok);
}

static void	_minishell_loop(void)
{
	char	*line;

	rl_outstream = stderr;
	make_map();
	setup_signal();
	while(1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		_analyze(line);
		free(line);
	}
	write(1, "exit\n", 5);
	exit(g_status.exit_status);
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
