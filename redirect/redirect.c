/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:21:28 by kishizu           #+#    #+#             */
/*   Updated: 2024/02/21 21:48:54 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect.h"

void	redirect_output(char *dest)
{
	int	out_fd;

	out_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644); // for_outfile;
	if (out_fd < 0)
		// open error
	dup2(out_fd, STDOUT_FILENO);
}

void	append_redirect_output(char *dest)
{
	int	out_fd;

	out_fd = open(dest, O_WRONLY | O_CREAT | O_APPEND, 0644); // for_outfile;
	if (out_fd < 0)
		// open error
	dup2(out_fd, STDOUT_FILENO);
}

void	redirect_input(char *src)
{
	int	in_fd;

	in_fd = open(src, O_RDONLY, 0644); // for_outfile;
	if (in_fd < 0)
		// open error
	dup2(in_fd, STDIN_FILENO);
}

void	redirect_heredoc(char *delimiter)
{
	int		tmpfile_fd;
	char	*oneline;

	tmpfile_fd = open("/tmp/.tmp_heredoc", (O_WRONLY | O_CREAT), 0666);
	while (1)
	{
		oneline = readline("> ");
		if (oneline == NULL || (((ft_strncmp(oneline, delimiter,
							ft_strlen(delimiter)) == 0))
				&& (oneline[ft_strlen(delimiter)] == '\n')))
			break ;
		if (oneline != NULL)
			write(tmpfile_fd, oneline, ft_strlen(oneline));
	}
	close(tmpfile_fd);
}
