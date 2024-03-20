/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:13:46 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 18:53:35 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static void	_show_declare(t_map *envmap)
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

bool	is_valid_export_variable(const char *variable_name)
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
			return (true);
		else if (!ft_isalpha((int)variable_name[i])
			&& !ft_isdigit((int)variable_name[i])
			&& variable_name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

// export KEY1=VALUE1 KEY2=VALUE2 ...
int	builtin_export(char **argv, t_map *envmap)
{
	int	status;
	int	i;

	status = SUCCESS;
	if (!argv[1])
	{
		_show_declare(envmap);
		return (SUCCESS);
	}
	i = 1;
	while (argv[i])
	{
		if (!is_valid_export_variable(argv[i]) || add_var(envmap, argv[1],
				true) == FAILURE)
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
