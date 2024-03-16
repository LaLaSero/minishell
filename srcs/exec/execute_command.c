/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:57:34 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 19:10:10 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
# include "../libft/libft.h"

static void	_show_error(char *command, char *message)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(2, command, ft_strlen(command));
	write(2, ": ", 2);
	write(2, message, ft_strlen(message));
}

static void	_free_str(char **path)
{
	int	i;

	i = 0;
	while (path[i] != NULL)
		free(path[i++]);
	free(path);
}

static int	_is_fullpath(char *command)
{
	if (ft_strchr(command, '/') != NULL)
	{
		if (access(command, F_OK | X_OK) != 0)
			return (2);
		return (1);
	}
	return (0);
}

// envpは、環境変数の配列、ターミナルでenvを打つ
// 項目ごとに格納されてる
static char	*_find_path_list(char **envp)
{
	int		i;
	char	*path_list;

	i = 0;
	path_list = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_list = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
			break ;
		}
		i++;
	}
	if (path_list == NULL)
	{
		return (NULL);
	}
	return (path_list);
}

//有効な(PATH)/commandを探す
// commandは1つだと仮定
// exit は勝手にclose()してくれる
static char	*find_accessible_path(char *path_list, char *command)
{
	int		i;
	char	*tmp;
	char	**path;
	char	*accessible_path;

	path = ft_split(path_list, ':');
	i = 0;
	while (path[i] != NULL)
	{
		tmp = ft_strjoin(path[i], "/");
		accessible_path = ft_strjoin(tmp, command);
		free(tmp);
		if (access(accessible_path, F_OK | X_OK) == 0)
			break ;
		free(accessible_path);
		i++;
	}
	if (path[i] == NULL)
	{
		_free_str(path);
		_show_error(command, "command not found\n");
		exit(127);
	}
	_free_str(path);
	return (accessible_path);
}

// envpは、環境変数の配列、ターミナルでenvを打つ
// commandの最初の部分だけaccess()に渡す
// char *cmd2_args[] = {"wc", "-l", NULL};
// execve("/usr/bin/wc", cmd2_args, envp);みたいな引数指定になる
// execve()は、成功したらfree,closeは不要
void	execute_command(char **command_splitted, char **envp)
{
	char	*path_list;
	char	*accessible_path;

	path_list = _find_path_list(envp);
	if (_is_fullpath(command_splitted[0]) == 1)
		accessible_path = ft_strdup(command_splitted[0]);
	else if (_is_fullpath(command_splitted[0]) == 2 || path_list == NULL)
	{
		_show_error(command_splitted[0], "No such file or directory\n");
		_free_str(command_splitted);
		free(path_list);
		exit(127);
	}
	else
		accessible_path = find_accessible_path(path_list, command_splitted[0]);
	free(path_list);
	if (execve(accessible_path, command_splitted, envp) == -1)
	{
		_free_str(command_splitted);
		free(accessible_path);
		perror("minishell: execve");
		exit(-1);
	}
}
