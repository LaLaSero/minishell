/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:43:31 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 17:35:04 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

t_var	*new_var(char *key, char *value)
{
	t_var	*item;

	item = ft_calloc(1, sizeof(*item));
	if (item == NULL)
		fatal_error("calloc");
	item->key = key;
	item->value = value;
	item->next = NULL;
	return (item);
}

t_map	*init_map(void)
{
	t_map	*map;

	map = ft_calloc(1, sizeof(*map));
	if (map == NULL)
		fatal_error("calloc");
	map->item_head.key = NULL;
	map->item_head.value = NULL;
	map->item_head.next = NULL;
	return (map);
}

void	make_map(t_map **envmap)
{
	extern char	**environ;
	char		cwd[PATH_MAX];
	char		*temp;
	t_map		*map;

	*envmap = init_map();
	(*envmap)->exit_status = 0;
	map = *envmap;
	while (*environ)
	{
		add_var(map, *environ, false);
		environ++;
	}
	if (get_value("PWD", map) == NULL)
	{
		getcwd(cwd, PATH_MAX);
		temp = ft_strjoin("PWD=", cwd);
		add_var(map, temp, false);
		free(temp);
	}
}
