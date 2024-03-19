/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 19:18:22 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/19 16:57:22 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

bool	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}

bool	is_metacharacter(char c)
{
	static const char	*metacharacters;
	size_t				i;

	metacharacters = "|<> \n";
	i = 0;
	while (metacharacters[i])
	{
		if (c == metacharacters[i])
			return (true);
		i++;
	}
	return (false);
}

bool	is_word(const char *s)
{
	if (!*s)
		return (false);
	if (is_metacharacter(*s))
		return (false);
	return (true);
}
