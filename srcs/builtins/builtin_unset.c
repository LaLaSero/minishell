/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:59:58 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/17 04:37:17 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h" 
# include "../libft/libft.h"

extern t_map *envmap;

static void	_show_unset_error(char *key)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, "unset: '", 8);
	write(STDERR_FILENO, key, ft_strlen(key));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
}

int	builtin_unset(char **argv)
{
	int	i;
	int	status;

	status = SUCCESS;
	i = 0;
	while (argv[i])
	{
		if (remove_var(envmap, argv[i]) == FAILURE)
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
