/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:13:46 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 20:33:03 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static void	_show_declare(void)
{
	extern t_status	g_status;
	t_var			*cur;

	cur = g_status.envmap->item_head.next;
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
	extern t_status	g_status;
	int				status;
	int				i;

	status = SUCCESS;
	if (!argv[1])
	{
		_show_declare();
		return (SUCCESS);
	}
	i = 0;
	while (argv[i])
	{
		if (add_var(g_status.envmap, argv[1], false) == FAILURE)
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
