/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:59:40 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 17:44:35 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

extern t_map	*envmap;

char	*get_value(char *key)
{
	t_var	*cur;

	if (key == NULL)
		return (NULL);
	cur = envmap->item_head.next;
	while (cur)
	{
		if (strncmp(key, cur->key, strlen(key) + 1) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

char	**get_environ(t_map *map)
{
	size_t	i;
	size_t	size;
	t_var	*cur;
	char	**environ;

	size = get_sizeof_map(map);
	environ = calloc(size + 1, sizeof(char *));
	i = 0;
	cur = map->item_head.next;
	while (cur)
	{
		environ[i] = get_full_sentence(cur);
		if (environ[i] == NULL)
			fatal_error("calloc");
		i++;
		cur = cur->next;
	}
	environ[i] = NULL;
	return (environ);
}

char	*get_full_sentence(t_var *var)
{
	size_t	strsize;
	char	*string;

	strsize = strlen(var->key) + 2;
	if (var->value)
		strsize += strlen(var->value);
	string = malloc(strsize);
	strlcpy(string, var->key, strsize);
	if (var->value)
	{
		strlcat(string, "=", strsize);
		strlcat(string, var->value, strsize);
	}
	return (string);
}
