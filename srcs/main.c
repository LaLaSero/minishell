/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:03:52 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 21:46:37 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/libft.h"
# include "../includes/minishell.h" 

t_status status = {};
t_map *envmap;

void	init_status(void)
{
	status.had_error = false;
	status.exit_status = 0;
	status.signal = 0;
	status.is_interrupted = false;
}

void	interpret(char *line)
{
	t_token	*tok;
	t_node	*node;

	status.had_error = false;
	tok = tokenize(line);
	if (tok->kind == TK_EOF)
		;
	else if(status.had_error)
		status.exit_status = ERROR_IN_TOKENIZE;
	else
	{
		node = parse(tok);
		if(status.had_error == true)
			status.exit_status = ERROR_IN_PARSE;
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

void	minishell()
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
		interpret(line);
		free(line);
	}
	write(1, "exit\n", 5);
	exit(status.exit_status);
}

int	main(int argc, char **argv)
{
	init_status();
	(void)argv;
	if (argc == 1)
		minishell();
	return (0);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }
