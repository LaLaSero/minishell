/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 13:03:31 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 14:24:00 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_env(t_map *envmap)
{
	extern t_status	g_status;
	t_var			*cur;

	cur = envmap->item_head.next;
	while (cur)
	{
		if (cur->value && cur->value[0] != '\0')
			printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	printf("_=/usr/bin/env\n");
	return (SUCCESS);
}
