/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/12 11:08:44 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

extern t_map *envmap;

void update_oldpwd(char *pwd_value)
{
	char *oldpwd_value;

	if (pwd_value == NULL)
		add_var(envmap, "OLDPWD=NULL", true);
	else
	{
		oldpwd_value = ft_strjoin("OLDPWD=", pwd_value);
		add_var(envmap, oldpwd_value, true);
		free(oldpwd_value);
	}
}

bool detect_target(char **path_loc, char *path, char *target)
{
	size_t	len;

	len = ft_strlen(target);
	if (ft_strncmp(path, target, len) == 0)
	{
		if (path[len] == '/' || path[len] == '\0')
		{
			*path_loc = path + len;
			return (true);
		}
	}
	return (false);
}

void remove_last_dir(char *pwd_value)
{
	size_t	len;

	len = ft_strlen(pwd_value);
	while (len > 0)
	{
		len--;
		if (pwd_value[len] == '/')
		{
			if (len > 0)
				pwd_value[len] = '\0';
			break;
		}
	}
}

void add_dir(char *new_pwd, char *path, char **path_loc)
{
	size_t	len;

	len = ft_strlen(new_pwd);
	if (len > 1 && new_pwd[len - 1] != '/')
		new_pwd[len++] = '/';
	while (*path && *path != '/')
		new_pwd[len++] = *path++;
	new_pwd[len] = '\0';
	*path_loc = path;
}

char *update_pwd_value(char *pwd_value, char *path)
{
	char newpwd_value[PATH_MAX];
	char *res;

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

int argv_to_path(char **argv, char *path)
{
	char *home_value;

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

int builtin_cd(char **argv)
{
	char *pwd_value;
	char *newpwd_value;
	char path[PATH_MAX];
	char *joined_newpwd;

	pwd_value = get_value("PWD");
	update_oldpwd(pwd_value);
	if (argv_to_path(argv, path) == FAILURE)
		return (FAILURE);
	printf("path: [%s]\n", path);
	if (chdir(path) < 0)
	{
		dprintf(STDERR_FILENO, "minishell: cd: %s: No such file or directory\n", argv[1]);
		return (FAILURE);
	}
	newpwd_value = update_pwd_value(pwd_value, path);
	joined_newpwd = ft_strjoin("PWD=", newpwd_value);
	add_var(envmap, joined_newpwd, true);
	free(joined_newpwd);
	return (SUCCESS);
}
