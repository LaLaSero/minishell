/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:09:00 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 20:29:39 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

void	update_oldpwd(char *pwd_value)
{
	extern t_status g_status	;
	char			*oldpwd_value;

	if (pwd_value == NULL)
		add_var(g_status.envmap, "OLDPWD", true);
	else
	{
		oldpwd_value = ft_strjoin("OLDPWD=", pwd_value);
		add_var(g_status.envmap, oldpwd_value, false);
		free(oldpwd_value);
	}
}

bool	detect_target(char **path_loc, char *path, char *target)
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

void	remove_last_dir(char *pwd_value)
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
			break ;
		}
	}
}

void	add_dir(char *new_pwd, char *path, char **path_loc)
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
