/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:53:30 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/25 00:13:42 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

size_t	get_sizeof_token(t_token *args)
{
	size_t	i;
	t_token	*tmp;

	i = 0;
	tmp = args;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	isbuiltin(t_node *command)
{
	if (ft_strncmp(command->args->word, "cd", 3) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "echo", 5) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "exit", 5) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "export", 7) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "pwd", 4) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "unset", 6) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "env", 4) == 0)
		return (true);
	return (false);
}

static bool	is_valid_fd(int fd)
{
	struct stat	st;

	if (fd < 0)
		return (false);
	errno = 0;
	if (fstat(fd, &st) < 0 && errno == EBADF)
		return (false);
	return (true);
}

int	stash_fd(int fd)
{
	int	stash;

	if (is_valid_fd(fd) == false)
	{
		fatal_error("stash error");
	}
	stash = 11;
	while (is_valid_fd(stash))
		stash++;
	stash = dup2(fd, stash);
	if (stash < 0)
		fatal_error("dup2 error");
	if (close(fd) < 0)
		fatal_error("close error");
	return (stash);
}

void	identify_dir_or_file(char **path)
{
		struct stat	path_stat;

		stat(path[0], &path_stat);
		if (S_ISDIR(path_stat.st_mode))
		{
			show_exec_error(path[0], "is a directory\n");
			free_argv(path);
			exit(126);
		}
}
