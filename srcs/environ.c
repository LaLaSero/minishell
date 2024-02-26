/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 23:59:40 by yutakagi          #+#    #+#             */
/*   Updated: 2024/02/26 19:33:40 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_map *envmap;

// void	perror_prefix(void)
// {
// 	dprintf(STDERR_FILENO, "%s", ERROR_PREFIX);
// }

// void	fatal_error(const char *msg)
// {
// 	perror_prefix();
// 	dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
// 	exit(1);
// }

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

t_map *init_map()
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

int remove_var(t_map *map, char *key)
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

int add_var(t_map *map, char *line, bool null_value)
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

void make_map(void)
{
	char cwd[PATH_MAX];
	extern char **environ;
	
	envmap = init_map();
	while (*environ)
	{
		add_var(envmap, *environ, false);
		environ++;
	}
	if (get_value("PWD") == NULL)
	{
		getcwd(cwd, PATH_MAX);
		map_set(map, "PWD", cwd);
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

// char *extract_key(char *line)
// {
// 	char *p;
// 	char *key;

// 	p = strchr(line, '=');
// 	if (p == NULL)
// 		return (strdup(line));
// 	key = strndup(line, p - line);
// 	if (key == NULL)
// 		fatal_error("strndup");
// 	return (key);
// }

// int main(int argc, char **argv)
// {
// 	char *key = extract_key(argv[1]);
// 	make_map();
// 	exe_export(argv);
// 	printf("[%s]=%s\n", key, get_value(key));
// 	free(key);
	
// }

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q a.out");
// }