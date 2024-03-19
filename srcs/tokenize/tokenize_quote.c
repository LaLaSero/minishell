/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 21:16:33 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 13:51:39 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static void	handle_single_quote(char *line, char **line_loc, int *error)
{
	extern t_status	g_status;

	line++;
	while (*line && *line != '\'')
		line++;
	if (*line == '\0')
	{
		*error = true;
		tokenize_error(line, line_loc);
		write(1, "Unclosed single quote\n", 23);
	}
	else
		line++;
	*line_loc = line;
}

static void	handle_double_quote(char *line, char **line_loc, int *error)
{
	extern t_status	g_status;

	line++;
	while (*line && *line != '\"')
		line++;
	if (*line == '\0')
	{
		*error = true;
		tokenize_error(line, line_loc);
		write(1, "Unclosed single quote\n", 23);
	}
	else
		line++;
	*line_loc = line;
}

t_token	*word(char **line_loc, char *line, int *error)
{
	extern t_status	g_status;
	const char		*start;
	char			*word;

	start = line;
	while (*line && !is_metacharacter(*line))
	{
		if (*line == '\'')
		{
			handle_single_quote(line, &line, error);
		}
		else if (*line == '\"')
		{
			handle_double_quote(line, &line, error);
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
