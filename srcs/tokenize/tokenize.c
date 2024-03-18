/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:48:07 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 21:09:57 by yutakagi         ###   ########.fr       */
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

t_token	*operator(char **line_loc, char *line)
{
	static char *const	operator_list[] = {">>", "<<", "<", ">", "|", "\n"};
	size_t				i;
	char				*op;
	extern t_status		g_status;

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
	assert_error("Unexpected operator");
	g_status.had_error = true;
	return (NULL);
}

static void	handle_single_quote(char *line, char **line_loc)
{
	extern t_status	g_status;

	line++;
	while (*line && *line != '\'')
		line++;
	if (*line == '\0')
	{
		g_status.had_error = true;
		tokenize_error(line, line_loc);
		write(1, "Unclosed single quote\n", 23);
	}
	else
		line++;
	*line_loc = line;
}

static void	handle_double_quote(char *line, char **line_loc)
{
	extern t_status	g_status;

	line++;
	while (*line && *line != '\"')
		line++;
	if (*line == '\0')
	{
		g_status.had_error = true;
		tokenize_error(line, line_loc);
		write(1, "Unclosed single quote\n", 23);
	}
	else
		line++;
	*line_loc = line;
}

t_token	*word(char **line_loc, char *line)
{
	extern t_status	g_status;
	const char		*start;
	char			*word;

	start = line;
	while (*line && !is_metacharacter(*line))
	{
		if (*line == '\'')
		{
			handle_single_quote(line, &line);
		}
		else if (*line == '\"')
		{
			handle_double_quote(line, &line);
		}
		else
			line++;
	}
	word = ft_substr(start, 0, line - start);
	if (word == NULL)
		fatal_error("substr");
	*line_loc = line;
	return (new_token(word, TK_WORD));
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

t_token	*tokenize(char *line)
{
	extern t_status	g_status;
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
			tok->next = operator(&line, line);
			tok = tok->next;
		}
		else if (is_word(line))
		{
			tok->next = word(&line, line);
			tok = tok->next;
		}
		else
		{
			assert_error("Unexpected Token");
			g_status.had_error = true;
		}
	}
	tok->next = new_token(NULL, TK_EOF);
	return (head.next);
}
