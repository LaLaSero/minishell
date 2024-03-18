/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:43:31 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 21:43:57 by kishizu          ###   ########.fr       */
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

void	make_map(void)
{
	extern char		**environ;
	extern t_status	g_status;
	char			cwd[PATH_MAX];
	char			*temp;

	g_status.envmap = init_map();
	while (*environ)
	{
		add_var(g_status.envmap, *environ, false);
		environ++;
	}
	if (get_value("PWD") == NULL)
	{
		getcwd(cwd, PATH_MAX);
		temp = ft_strjoin("PWD=", cwd);
		add_var(g_status.envmap, temp, false);
		free(temp);
	}
}
