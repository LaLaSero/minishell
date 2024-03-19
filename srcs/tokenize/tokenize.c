/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:48:07 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 17:19:44 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"
#include <assert.h>

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = ft_calloc(1, sizeof(*tok));
	if (tok == NULL)
		fatal_error("calloc");
	tok->word = word;
	tok->kind = kind;
	return (tok);
}

t_token	*operator(char **line_loc, char *line, int *error)
{
	static char *const	operator_list[] = {">>", "<<", "<", ">", "|", "\n"};
	size_t				i;
	char				*op;

	i = 0;
	while (i < sizeof(operator_list) / sizeof(*operator_list))
	{
		if (ft_strncmp(line, operator_list[i],
				ft_strlen(operator_list[i])) == 0)
		{
			op = ft_strdup(operator_list[i]);
			if (op == NULL)
				fatal_error("strdup");
			*line_loc = line + ft_strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	assert_error("Unexpected operator", error);
	*error = true;
	return (NULL);
}

static int	_delete_space(char **line_loc, char *line)
{
	if (is_space(*line))
	{
		while (*line && is_space(*line))
			line++;
		*line_loc = line;
		return (true);
	}
	*line_loc = line;
	return (false);
}

t_token	*tokenize(char *line, int *error)
{
	t_token			head;
	t_token			*tok;

	head.next = NULL;
	tok = &head;
	while (*line)
	{
		if (_delete_space(&line, line) == true)
			continue ;
		if (is_metacharacter(*line))
		{
			tok->next = operator(&line, line, error);
			tok = tok->next;
		}
		else if (is_word(line))
		{
			tok->next = word(&line, line, error);
			tok = tok->next;
		}
		else
			*error = true;
	}
	tok->next = new_token(NULL, TK_EOF);
	return (head.next);
}
