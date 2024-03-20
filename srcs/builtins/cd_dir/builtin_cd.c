/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 17:51:12 by kishizu          ###   ########.fr       */
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

static char	*_update_pwd_value(char *pwd_value, char *path)
{
	char	newpwd_value[PATH_MAX];
	char	*res;

	if (*path == '/' || pwd_value == NULL)
		ft_strlcpy(newpwd_value, path, PATH_MAX);
	else
		ft_strlcpy(newpwd_value, pwd_value, PATH_MAX);
	while (*path)
	{
		if (*path == '/')
			path++;
		else if (detect_target(&path, path, "."))
			;
		else if (detect_target(&path, path, ".."))
			remove_last_dir(newpwd_value);
		else
			add_dir(newpwd_value, path, &path);
	}
	res = ft_strdup(newpwd_value);
	return (res);
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
		ft_strlcpy(path, home_value, PATH_MAX);
	}
	else
		ft_strlcpy(path, argv[1], PATH_MAX);
	return (SUCCESS);
}

int	builtin_cd(char **argv, t_map *envmap)
{
	char	*pwd_value;
	char	*newpwd_value;
	char	path[PATH_MAX];
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
		newpwd_value = _update_pwd_value(pwd_value, path);
		joined_newpwd = ft_strjoin("PWD=", newpwd_value);
		add_var(envmap, joined_newpwd, true);
		free(joined_newpwd);
	}
	return (SUCCESS);
}
