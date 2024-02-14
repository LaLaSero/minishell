/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 16:24:01 by yutakagi          #+#    #+#             */
/*   Updated: 2024/02/14 00:17:31 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	display_open_error(char *str, int fd)
{
	write(2, str, ft_strlen(str));
	close(fd);
	exit(-1);
}

// プロセスが終了するとexit(-1)したときに、close()が呼ばれる
// そのため、close()を呼ぶ必要がない
// execve()が成功したとき、そこでプロセスが終了するのでfree,closeは不要
static void	child_func(int infile_fd, int pipe_fd[2], char **argv, char **envp)
{
	close(pipe_fd[0]);
	dup2(infile_fd, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	execute_command(&argv[2], envp);
	exit(-1);
}

static void	parent_func(int outfile_fd,
			int pipe_fd[2], char **argv, char **envp)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	execute_command(&argv[3], envp);
	exit(-1);
}

// fork()の返り値は、親プロセスには子プロセスのプロセスIDが、子プロセスには0が返る
void	pipex(int infile_fd, int outfile_fd, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	process_id;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(-1);
	}
	process_id = fork();
	if (process_id == 0)
		child_func(infile_fd, pipe_fd, argv, envp);
	else if (process_id > 0)
	{
		waitpid(process_id, NULL, WNOHANG);
		parent_func(outfile_fd, pipe_fd, argv, envp);
	}
	else
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		perror("fork");
		exit(-1);
	}
}

// int	main(int argc, char **argv, char **envp)
// {
// 	int	infile_fd;
// 	int	outfile_fd;

// 	if (argc != 5)
// 	{
// 		write(2, "invalid argument\n", 17);
// 		return (-1);
// 	}
// 	infile_fd = open(argv[1], O_RDONLY);
// 	if (infile_fd == -1)
// 		display_open_error(strerror(errno), infile_fd);
// 	outfile_fd = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
// 	if (outfile_fd == -1)
// 		display_open_error(strerror(errno), outfile_fd);
// 	pipex(infile_fd, outfile_fd, argv, envp);
// 	close(infile_fd);
// 	close(outfile_fd);
// 	return (0);
// }
