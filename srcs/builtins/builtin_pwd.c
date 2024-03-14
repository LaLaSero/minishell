/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 19:24:01 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/14 20:30:35 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

bool is_correct_cwd(char *pwd)
{
	struct stat stat_pwd, stat_cwd;

	if (stat(pwd, &stat_pwd) != 0)
	{
		return false;
	}
	if (stat(".", &stat_cwd) != 0)
	{
		return false;
	}
	return (stat_pwd.st_ino == stat_cwd.st_ino
			&& stat_pwd.st_dev == stat_cwd.st_dev);
}

int builtin_pwd(void)
{
	char	*pwd;
	char	cwd[PATH_MAX];

	pwd = get_value("PWD");
	if (pwd == NULL || is_correct_cwd(pwd) == false)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			printf("cwd is NULL\n");
			return (FAILURE);
		}
		write(STDOUT, cwd, ft_strlen(cwd));
		write(STDOUT, "\n", 1);
		return (SUCCESS);
	}
	else
	{
		write(STDOUT, pwd, ft_strlen(pwd));
		write(STDOUT, "\n", 1);
		return (SUCCESS);
	}
}
