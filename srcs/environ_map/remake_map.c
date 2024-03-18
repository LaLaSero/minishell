/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remake_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:48:06 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 19:40:22 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static void	_realloc_var(t_var *var, char *value)
{
	free(var->value);
	if (!value)
		var->value = NULL;
	else
	{
		var->value = ft_strdup(value);
		if (var->value == NULL)
			fatal_error("strdup");
	}
}

static t_var	*_make_new_var(char *key, char *value)
{
	if (!value)
		return (new_var(ft_strdup(key), NULL));
	else
		return (new_var(ft_strdup(key), ft_strdup(value)));
}

int	remake_map(t_map *map, char *key, char *value)
{
	t_var	*cur;

	if (key == NULL)
		return (FAILURE);
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strncmp(key, cur->key, ft_strlen(key)) == 0)
			break ;
		cur = cur->next;
	}
	if (cur)
		_realloc_var(cur, value);
	else
	{
		cur = _make_new_var(key, value);
		if ((value == NULL && cur->value == NULL)
			|| (value != NULL && (cur->key == NULL || cur->value == NULL)))
			fatal_error("new_var");
		cur->next = map->item_head.next;
		map->item_head.next = cur;
	}
	return (SUCCESS);
}
