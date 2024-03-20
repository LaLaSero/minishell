/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:19 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 17:31:47 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

void	append_single_quote(char **dst, char **rest, char *p, int *error)
{
	append_char(dst, *p++);
	while (*p != SINGLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed single quote", error);
		append_char(dst, *p++);
	}
	append_char(dst, *p++);
	*rest = p;
}

void	append_double_quote(char **dst, char **rest, int *error, t_map *envmap)
{
	char	*p;

	p = *rest;
	append_char(dst, *p++);
	while (*p != DOUBLE_QUOTE_CHAR)
	{
		if (*p == '\0')
			assert_error("Unclosed double quote", error);
		else if (is_macro(p))
			expand_macro(dst, &p, p, envmap->exit_status);
		else if (is_variable(p))
			expand_variable_str(dst, &p, error, envmap);
		else
			append_char(dst, *p++);
	}
	append_char(dst, *p++);
	*rest = p;
}

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += ft_strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		ft_strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}
