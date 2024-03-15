/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:13:46 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 02:29:39 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern t_map *envmap;

static void	_show_declare(void)
{
	t_var	*cur;

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
int	builtin_export(char **argv)
{
	int	i;
	int	status;

	status = SUCCESS;
	if (!argv[1])
	{
		_show_declare();
		return (SUCCESS);
	}
	i = 0;
	while (argv[i])
	{
		if (add_var(envmap, argv[1], false) == FAILURE)
		{
			dprintf(STDERR_FILENO, "export: '%s': not a valid identifier\n", *argv);
			status = 255;
		}
		i++;
	}
	return (status);
}
