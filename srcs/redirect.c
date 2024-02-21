/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 19:18:43 by yutakagi          #+#    #+#             */
/*   Updated: 2024/02/19 23:06:34 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/libft.h"
# include "../includes/minishell.h" 
# include "../includes/pipex.h"

void	perror_prefix(void)
{
	dprintf(STDERR_FILENO, "%s", ERROR_PREFIX);
}

void	fatal_error(const char *msg)
{
	perror_prefix();
	dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
	exit(1);
}

int duplicate_fd(int fd)
{
	int newfd;

	newfd = fcntl(fd, F_DUPFD, 10);
	if (newfd < 0)
		fatal_error("dup");
	if (close(fd) < 0)
		fatal_error("close");
	return (newfd);
}


void redirect(int targetfd, char *filename)
{
	int fd;
	int stashed_target_fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		fatal_error("open");
	fd = duplicate_fd(fd);
	stashed_target_fd = duplicate_fd(targetfd);
	if (fd != targetfd)
	{
		if (dup2(fd, targetfd) < 0)
			fatal_error("dup2");
		close(fd);
	}
	// exec_commnd();
	write(1, "hello\n", 6);
	dup2(stashed_target_fd, targetfd);
}

int main(int argc, char **argv)
{
	(void)argv;
	// int fd = open("sample.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (argc == 1)
	{
		redirect(1, "test.txt");
	}
	// write(1, "hello\n", 6);
	// close(fd);
	return (0);
}