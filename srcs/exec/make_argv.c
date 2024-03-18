/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 19:06:22 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 22:52:31 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

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

bool	is_only_space(char *s)
{
	while (*s)
	{
		if (*s != ' ')
			return (false);
		s++;
	}
	return (true);
}

// "ls -a"を"ls", "-a"に分けてくっつける
void	implement_argv(t_token *args)
{
	t_token	*next_temp;
	char	**splitted;
	int		i;

	splitted = NULL;
	if (args->word && !is_only_space(args->word) && ft_strchr(args->word, ' '))
	{
		splitted = ft_split(args->word, ' ');
		if (splitted == NULL)
			fatal_error("split");
		args->word = ft_strdup(splitted[0]);
		next_temp = args->next;
		i = 1;
		while (splitted[i])
		{
			args->next = new_token(ft_strdup(splitted[i]), TK_WORD);
			args = args->next;
			i++;
		}
		args->next = next_temp;
		free_argv(splitted);
	}
}

char	**convert_to_argv(t_token *args)
{
	int		i;
	char	**argv;
	t_token	*tmp;
	t_token	*temp;

	temp = args;
	implement_argv(args);
	i = get_sizeof_token(temp);
	argv = ft_calloc(i + 1, sizeof(*argv));
	if (argv == NULL)
		fatal_error("calloc");
	i = 0;
	tmp = temp;
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
