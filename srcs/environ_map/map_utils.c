/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:32:21 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 18:40:59 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

extern t_map	*envmap;
int				reload_map(t_map *map, char *key, char *value);

int	add_var(t_map *map, char *line, bool null_value)
{
	char	*key;
	char	*value;
	char	*p;
	int		is_success;

	key = NULL;
	value = NULL;
	p = strchr(line, '=');
	if (p == NULL && null_value == false)
		fatal_error("Invalid environment variable");
	else if(p == NULL)
	{
		key = strdup(line);
		if (key == NULL)
			fatal_error("strdup");
		value = NULL;
	}
	else
	{
		key = strndup(line, p - line);
		if (key == NULL)
			fatal_error("strndup");
		value = strdup(p + 1);
		if (value == NULL)
			fatal_error("strdup");
	}
	is_success = reload_map(map, key, value);
	free(key);
	free(value);
	return (is_success);
}

int remove_var(t_map *map, char *key)
{
	t_var	*cur;
	t_var	*prev;

	if (key == NULL)
		return (FAILURE);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (strncmp(key, cur->key, strlen(key) + 1) == 0)
		{
			prev->next = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return (SUCCESS);
		}
		prev = cur;
		cur = cur->next;
	}
	return (FAILURE);
}

size_t	get_sizeof_map(t_map *map)
{
	size_t	i;
	t_var	*cur;

	i = 0;
	cur = map->item_head.next;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

int	reload_map(t_map *map, char *key, char *value)
{
	t_var	*cur;

	if (key == NULL)
		return (FAILURE);
	cur = map->item_head.next;
	while (cur)
	{
		if (strncmp(key, cur->key, strlen(key)) == 0)
			break ;
		cur = cur->next;
	}
	if (cur)
	{
		free(cur->value);
		if (!value)
			cur->value = NULL;
		else
		{
			cur->value = strdup(value);
			if (cur->value == NULL)
				fatal_error("strdup");
		}
	}
	else
	{
		if (value == NULL)
		{
			cur = new_var(strdup(key), NULL);
			if (cur->value == NULL)
				fatal_error("new_var");
		}
		else
		{
			cur = new_var(strdup(key), strdup(value));
			if (cur->key == NULL || cur->value == NULL)
				fatal_error("new_var");
		}
		cur->next = map->item_head.next;
		map->item_head.next = cur;
	}
	return (SUCCESS);
}
