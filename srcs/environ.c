/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:59:40 by yutakagi          #+#    #+#             */
/*   Updated: 2024/02/22 01:56:53 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_map *envmap;

t_var *new_var(char *key, char *value)
{
	t_var *item;

	item = calloc(1, sizeof(*item));
	if (item == NULL)
		fatal_error("calloc");
	item->key = key;
	item->value = value;
	item->next = NULL;
	return (item);
}

t_map *make_map()
{
	t_map *map;

	map = calloc(1, sizeof(*map));
	if (map == NULL)
		fatal_error("calloc");
	map->item_head.key = NULL;
	map->item_head.value = NULL;
	map->item_head.next = NULL;
	return (map);
}

int free_map(t_map *map, char *key)
{
	t_var *cur;
	t_var *prev;

	if (key == NULL)
		return (FAILURE);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (strncmp(key, cur->key, strlen(key)) == 0)
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

int reload_map(t_map *map, char *key, char *value)
{
	t_var *cur;

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
		if(!value)
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

void add_var(t_map *map, char *line, bool null_value)
{
	char *key;
	char *value;
	char *p;
	int is_success;

	p = strchr(line, '=');
	if (p == NULL)
		fatal_error("Invalid environment variable");
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

t_var *make_list_from_env(void)
{
	extern char **environ;
	
	envmap = make_map();
	while (*environ)
	{
		add_var(envmap, *environ, false);
		environ++;
	}
}
char *get_value(char *key)
{
	t_var *cur;

	if (key == NULL)
		return (NULL);
	cur = envmap->item_head.next;
	while (cur)
	{
		if (strncmp(key, cur->key, strlen(key)) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}
