/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:13:46 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 17:10:37 by yutakagi         ###   ########.fr       */
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

	i = 0;
	if (variable_name == NULL || *variable_name == '\0'
		|| *variable_name == '=')
		return (false);
	if (ft_isdigit((unsigned char)*variable_name))
		return (false);
	while (variable_name[i])
	{
		if (*variable_name == '=')
			return (true);
		if (!ft_isalpha((unsigned char)*variable_name)
			&& !ft_isdigit((unsigned char)*variable_name)
			&& *variable_name != '_')
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
	i = 0;
	while (argv[i])
	{
		if (!is_valid_export_variable(argv[i]) || add_var(envmap, argv[1],
				false) == FAILURE)
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
