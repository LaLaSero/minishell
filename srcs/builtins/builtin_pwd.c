/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 19:24:01 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/08 01:35:07 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

// stat関数使って現在のディレクトリとしているパスと
// 環境変数のPWDが一致しているか確認する関数を作成する
int builtin_pwd(void)
{
	char *path;

	path = get_value("PWD");
	if (path == NULL)
	{
		ft_putendl_fd("minishell: pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory", STDERR);
		return (FAILURE);
	}
	else
	{
		write(STDOUT, path, ft_strlen(path));
		write(STDOUT, "\n", 1);
		return (SUCCESS);
	}
}