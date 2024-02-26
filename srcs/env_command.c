/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 13:03:31 by yutakagi          #+#    #+#             */
/*   Updated: 2024/02/23 13:05:49 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern t_map *envmap;

int	env_command()
{
	t_var	*cur;

	cur = envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
			printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	_printf("_=/usr/bin/env");
	return (SUCCESS);
}