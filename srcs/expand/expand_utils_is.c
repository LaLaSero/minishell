/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_is.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishizu <kishizu@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:56:10 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 16:16:50 by kishizu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
#include "../libft/libft.h"

bool	is_alpha_under(char c)
{
	return (ft_isalpha(c) || c == '_');
}

bool	is_alpha_num_under(char c)
{
	return (is_alpha_under(c) || ft_isdigit(c));
}

bool	is_identifier(const char *s)
{
	if (!is_alpha_under(*s))
		return (false);
	s++;
	while (*s)
	{
		if (!is_alpha_num_under(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	is_variable(char *s)
{
	return (s[0] == '$' && (is_alpha_under(s[1]) || s[1] == '?'));
}

bool	is_macro(char *s)
{
	return ((s[0] == '$' && s[1] == '?'));
}
