/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:13:46 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 14:17:24 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static void	_show_declare(t_map *envmap)
{
	// extern t_status	g_status;
	t_var			*cur;

	cur = envmap->item_head.next;
	while (cur)
	{
		printf("declare -x %s", cur->key);
		if (cur->value)
			printf("=\"%s\"", cur->value);
		printf("\n");
		cur = cur->next;
	}
}

// export KEY1=VALUE1 KEY2=VALUE2 ...
int	builtin_export(char **argv, t_map *envmap)
{
	// extern t_status	g_status;
	int				status;
	int				i;

	status = SUCCESS;
	if (!argv[1])
	{
		_show_declare(envmap);
		return (SUCCESS);
	}
	i = 0;
	while (argv[i])
	{
		if (add_var(envmap, argv[1], false) == FAILURE)
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO, "export: '", 9);
			write(STDERR_FILENO, argv[i], ft_strlen(argv[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 26);
			status = 255;
		}
		i++;
	}
	return (status);
}
