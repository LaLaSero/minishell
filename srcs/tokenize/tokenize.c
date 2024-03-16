/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:48:07 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 21:30:46 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"
#include <assert.h>

extern t_status	status;

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = calloc(1, sizeof(*tok));
	if (tok == NULL)
		fatal_error("calloc");
	tok->word = word;
	tok->kind = kind;
	return (tok);
}

t_token	*operator(char **line_loc, char *line)
{
	static char *const	operator_list[] = {">>", "<<", "||", "&&", ";;", "<", ">",
		"&", ";", "(", ")", "|", "\n"};
	size_t				i;
	char				*op;

	i = 0;
	while (i < sizeof(operator_list) / sizeof(*operator_list))
	{
		if (strncmp(line, operator_list[i], ft_strlen(operator_list[i])) == 0)
		{
			op = strdup(operator_list[i]);
			if (op == NULL)
				fatal_error("strdup");
			*line_loc = line + ft_strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	assert_error("Unexpected operator");
	status.had_error = true;
	return (NULL);
}


t_token *word(char **line_loc, char *line) {
	const char	*start;
	char		*word;

	start = line;
	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR)
		{
			line++;
			while (*line && *line != SINGLE_QUOTE_CHAR)
				line++;
			if (*line == '\0')
			{
				fatal_error("Unclosed single quote");
				// exit(1);
				status.had_error = true;
				tokenize_error(line, line_loc);
			}
			else
				line++;
		}
		else if (*line == DOUBLE_QUOTE_CHAR)
		{
			line++;
			while (*line && *line != DOUBLE_QUOTE_CHAR)
				line++;
			if (*line == '\0')
			{
				fatal_error("Unclosed double quote");
				exit(1);
			}
			else
				line++;
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

int	delete_space(char **line_loc, char *line)
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
	t_token	head;
	t_token	*tok;

	head.next = NULL;
	tok = &head;
	while (*line)
	{
		if (delete_space(&line, line) == true)
			continue;
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
			status.had_error = true;
		}
	}
	tok->next = new_token(NULL, TK_EOF);
	return (head.next);
}
