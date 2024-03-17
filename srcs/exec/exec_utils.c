/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:53:30 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/17 20:09:43 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

int	get_sizeof_token(t_token *args)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = args;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**convert_to_argv(t_token *args)
{
	int		i;
	char	**argv;
	t_token	*tmp;

	i = get_sizeof_token(args);
	argv = ft_calloc(i + 1, sizeof(*argv));
	if (argv == NULL)
		fatal_error("calloc");
	i = 0;
	tmp = args;
	while (tmp)
	{
		if (tmp->word != NULL)
			argv[i] = ft_strdup(tmp->word);
		tmp = tmp->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

int	isbuiltin(t_node *command)
{
	if (ft_strncmp(command->args->word, "cd", 3) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "echo", 5) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "exit", 5) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "export", 7) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "pwd", 4) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "unset", 6) == 0)
		return (true);
	else if (ft_strncmp(command->args->word, "env", 4) == 0)
		return (true);
	return (false);
}

int	stash_fd(int fd)
{
	int	stash;

	stash = fcntl(fd, F_DUPFD, 10);
	if (stash < 0)
	{
		fatal_error("stash error");
		return (FAILURE);
	}
	if (close(fd) < 0)
	{
		fatal_error("close error");
		return (FAILURE);
	}
	return (stash);
}
