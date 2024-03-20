/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 20:10:56 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 17:44:25 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

void	write_user_input_to_pipe(char *delimiter, int pipefd[2])
{
	char			*line;
	extern int		g_signal;

	while (1)
	{
		if (g_signal == 256)
			g_signal = 0;
		line = readline("> ");
		if (line == NULL
			|| !delimiter
			|| *delimiter == '\0'
			|| ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0
			|| g_signal == 256)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
}

int	open_heredoc(char *delimiter)
{
	int				pipefd[2];
	extern int		g_signal;

	if (pipe(pipefd) < 0)
	{
		fatal_error("pipe error");
		return (FAILURE);
	}
	write_user_input_to_pipe(delimiter, pipefd);
	if (close(pipefd[1]) < 0)
	{
		fatal_error("close error");
		return (FAILURE);
	}
	if (g_signal == 256)
	{
		g_signal = 0;
		if (close(pipefd[0]) < 0)
		{
			fatal_error("close error");
			return (FAILURE);
		}
		return (FAILURE);
	}
	return (pipefd[0]);
}
