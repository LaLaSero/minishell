/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:43:31 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/17 20:18:26 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

extern t_map	*envmap;

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
	char		cwd[PATH_MAX];
	char 		*temp;
	extern char	**environ;

	envmap = init_map();
	while (*environ)
	{
		add_var(envmap, *environ, false);
		environ++;
	}
	if (get_value("PWD") == NULL)
	{
		getcwd(cwd, PATH_MAX);
		temp = ft_strjoin("PWD=", cwd);
		add_var(envmap, temp, false);
		free(temp);
	}
}