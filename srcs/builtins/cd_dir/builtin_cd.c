/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 17:10:25 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

extern t_map *envmap;

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

static int	_argv_to_path(char **argv, char *path)
{
	char	*home_value;

	if (argv[1] == NULL)
	{
		home_value = get_value("HOME");
		if (home_value == NULL)
		{
			dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return (FAILURE);
		}
		ft_strlcpy(path, home_value, PATH_MAX);
	}
	else
		ft_strlcpy(path, argv[1], PATH_MAX);
	return (SUCCESS);
}

int	builtin_cd(char **argv)
{
	char	*pwd_value;
	char	*newpwd_value;
	char	path[PATH_MAX];
	char	*joined_newpwd;

	pwd_value = get_value("PWD");
	update_oldpwd(pwd_value);
	if (_argv_to_path(argv, path) == FAILURE)
		return (1);
	if (chdir(path) < 0)
	{
		dprintf(STDERR_FILENO, "minishell: cd: %s: No such file or directory\n", argv[1]);
		return (1);
	}
	newpwd_value = _update_pwd_value(pwd_value, path);
	joined_newpwd = ft_strjoin("PWD=", newpwd_value);
	add_var(envmap, joined_newpwd, true);
	free(joined_newpwd);
	return (SUCCESS);
}