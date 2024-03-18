/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 21:16:33 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/18 21:21:49 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

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
