/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/23 17:11:04 by yutakagi         ###   ########.fr       */
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

static bool	is_valid_unset_variable(const char *variable_name)
{
	int	i;

	if (variable_name == NULL || *variable_name == '\0'
		|| *variable_name == '=')
		return (false);
	if (ft_isdigit((int)*variable_name))
		return (false);
	i = 1;
	while (variable_name[i])
	{
		if (variable_name[i] == '=')
			return (false);
		else if (!ft_isalpha((int)variable_name[i])
			&& !ft_isdigit((int)variable_name[i])
			&& variable_name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	builtin_unset(char **argv, t_map *envmap)
{
	int	status;
	int	i;

	status = SUCCESS;
	i = 0;
	while (argv[i])
	{
		if (is_valid_unset_variable(argv[i]) == false)
		{
			status = 1;
			_show_unset_error(argv[i]);
			i++;
			continue ;
		}
		else if (remove_var(envmap, argv[i]) == FAILURE)
		{
			status = 0;
		}
		else
			status = 0;
		i++;
	}
	return (status);
}
