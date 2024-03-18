/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 21:41:52 by kishizu          ###   ########.fr       */
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

int	builtin_unset(char **argv)
{
	extern t_status	g_status;
	int				status;
	int				i;

	status = SUCCESS;
	i = 0;
	while (argv[i])
	{
		if (remove_var(g_status.envmap, argv[i]) == FAILURE)
		{
			_show_unset_error(argv[i]);
			status = 255;
		}
		else
			status = 255;
		i++;
	}
	return (status);
}
