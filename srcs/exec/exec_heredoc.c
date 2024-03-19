/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 20:10:56 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 18:29:00 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

void	write_user_input_to_pipe(char *delimiter, int pipefd[2])
{
	char			*line;
	extern t_status	g_status;

	while (1)
	{
		line = readline("> ");
		if (line == NULL
			|| *line == '\0'
			|| !delimiter
			|| *delimiter == '\0'
			|| ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			|| g_status.is_interrupted == true)
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
	extern t_status	g_status;

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
	if (g_status.is_interrupted == true)
	{
		if (close(pipefd[0]) < 0)
		{
			fatal_error("close error");
			return (FAILURE);
		}
		return (FAILURE);
	}
	return (pipefd[0]);
}
