/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/23 16:34:55 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

static void	_show_unset_error(char *key)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, "unset: '", 8);
	write(STDERR_FILENO, key, ft_strlen(key));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
}

int	builtin_unset(char **argv, t_map *envmap)
{
	int	status;
	int	i;

	status = SUCCESS;
	i = 0;
	while (argv[i])
	{
		if (is_valid_export_variable(argv[i]) == false)
		{
			status = 1;
			_show_unset_error(argv[i]);
			i++;
			continue ;
		}
		else if (remove_var(envmap, argv[i]) == FAILURE)
		{
			_show_unset_error(argv[i]);
			status = 0;
		}
		else
			status = 0;
		i++;
	}
	return (status);
}
