/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:32:21 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 18:43:07 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

extern t_map	*envmap;
int				remake_map(t_map *map, char *key, char *value);

char	*ft_strndup(char *s, size_t n)
{
	char	*str;
	size_t	i;

	str = ft_calloc(n + 1, sizeof(char));
	if (str == NULL)
		fatal_error("calloc");
	i = 0;
	while (i < n)
	{
		str[i] = s[i];
		i++;
	}
	return (str);
}

static int	_wrapped_remake_map(t_map *map, char *key, char *value)
{
	int	is_success;

	is_success = remake_map(map, key, value);
	free(key);
	free(value);
	return (is_success);
}

int	add_var(t_map *map, char *line, bool null_value)
{
	char	*key;
	char	*value;
	char	*p;

	value = NULL;
	key = NULL;
	p = ft_strchr(line, '=');
	if (p == NULL && null_value == false)
		fatal_error("Invalid environment variable");
	else if (p == NULL)
	{
		key = ft_strdup(line);
		if (key == NULL)
			fatal_error("strdup");
	}
	else
	{
		key = ft_strndup(line, p - line);
		if (key == NULL)
			fatal_error("strndup");
		value = ft_strdup(p + 1);
		if (value == NULL)
			fatal_error("strdup");
	}
	return (_wrapped_remake_map(map, key, value));
}

int	remove_var(t_map *map, char *key)
{
	t_var	*cur;
	t_var	*prev;

	if (key == NULL)
		return (FAILURE);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strncmp(key, cur->key, ft_strlen(key) + 1) == 0)
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
