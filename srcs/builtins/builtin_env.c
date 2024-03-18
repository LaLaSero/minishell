/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 13:03:31 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 21:39:44 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_env(void)
{
	extern t_status	g_status;
	t_var			*cur;

	cur = g_status.envmap->item_head.next;
	while (cur)
	{
		if (cur->value && cur->value[0] != '\0')
			printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	printf("_=/usr/bin/env\n");
	return (SUCCESS);
}
