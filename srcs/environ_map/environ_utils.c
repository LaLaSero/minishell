/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:59:40 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 16:15:47 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

char	*get_value(char *key, t_map *envmap)
{
	t_var	*cur;

	if (key == NULL)
		return (NULL);
	if (ft_strncmp(key, "?", 2) == 0)
		return (ft_itoa(envmap->exit_status));
	cur = envmap->item_head.next;
	while (cur)
	{
		if (ft_strncmp(key, cur->key, ft_strlen(key) + 1) == 0)
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
	environ = ft_calloc(size + 1, sizeof(char *));
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

	strsize = ft_strlen(var->key) + 2;
	if (var->value)
		strsize += ft_strlen(var->value);
	string = malloc(strsize);
	ft_strlcpy(string, var->key, strsize);
	if (var->value)
	{
		ft_strlcat(string, "=", strsize);
		ft_strlcat(string, var->value, strsize);
	}
	return (string);
}
