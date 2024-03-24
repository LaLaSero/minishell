/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/24 15:34:55 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

static void	_show_cd_error(char *message)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, "cd: ", 4);
	write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, "\n", 1);
}

static int	_argv_to_path(char **argv, char *path, t_map *envmap)
{
	char	*home_value;

	if (argv[1] == NULL)
	{
		home_value = get_value("HOME", envmap);
		if (home_value == NULL)
		{
			_show_cd_error("HOME not set");
			return (FAILURE);
		}
		if (ft_strlen(home_value) >= PATH_MAX - 7)
		{
			_show_cd_error("File name too long");
			return (FAILURE);
		}
		ft_strlcpy(path, home_value, PATH_MAX);
	}
	else if (ft_strlen(argv[1]) >= PATH_MAX - 7)
	{
		_show_cd_error("Path too long");
		return (FAILURE);
	}
	else
		ft_strlcpy(path, argv[1], PATH_MAX);
	return (SUCCESS);
}

int	builtin_cd(char **argv, t_map *envmap)
{
	char	*pwd_value;
	char	path[ARG_MAX];
	char	*joined_newpwd;

	pwd_value = get_value("PWD", envmap);
	if (_argv_to_path(argv, path, envmap) == FAILURE)
		return (1);
	if (chdir(path) < 0)
	{
		_show_cd_error("No such file or directory");
		return (1);
	}
	if (pwd_value)
	{
		getcwd(path, ARG_MAX);
		joined_newpwd = ft_strjoin("PWD=", path);
		if (joined_newpwd == NULL)
			fatal_error("malloc error");
		add_var(envmap, joined_newpwd, true);
		free(joined_newpwd);
	}
	return (SUCCESS);
}
