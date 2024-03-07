/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/08 02:12:10 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

extern t_map *envmap;

void reload_pwd_value(char *pwd_value)
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

void builtin_cd(char **argv)
{
	char *pwd_value;
	char *path[PATH_MAX];

	pwd_value = get_value("PWD");
	reload_oldpwd_value(pwd_value);
	if (argv[1] == NULL)
	{
		if (chdir(get_value("HOME")) == -1)
			dprintf(STDERR_FILENO, "minishell: cd: %s: No such file or directory\n", get_value("HOME"));
	}
	else if (chdir(argv[1]) == -1)
		dprintf(STDERR_FILENO, "minishell: cd: %s: No such file or directory\n", argv[1]);
	else
	{
		getcwd(path, PATH_MAX);
		add_var(envmap, ft_strjoin("PWD=", path), true);
	}
	
}